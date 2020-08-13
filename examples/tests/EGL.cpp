#include "EGL.h"

#ifndef WIN32

static struct {
	struct gbm_device *dev;
	struct gbm_surface *surface;
} gbm;

static struct {
	int fd;
	uint32_t ndisp;
	uint32_t crtc_id[MAX_DISPLAYS];
	uint32_t connector_id[MAX_DISPLAYS];
	uint32_t resource_id;
	uint32_t encoder[MAX_DISPLAYS];
	drmModeModeInfo *mode[MAX_DISPLAYS];
	drmModeConnector *connectors[MAX_DISPLAYS];
} drm;

struct drm_fb {
	struct gbm_bo *bo;
	uint32_t fb_id;
};

struct gbm_bo *bo;
void EGL::init()
{
	drmEventContext evctx = {
		.version = DRM_EVENT_CONTEXT_VERSION,
		.vblank_handler = vblank_handler,
		.page_flip_handler = page_flip_handler,
	};

	auto ret = initDRM();
	if (ret != 0)
	{
		printf("failed to initialize DRM\n");
		return ret;
	}
	printf("### Primary display => ConnectorId = %d, Resolution = %dx%d\n", drm.connector_id[DISP_ID], drm.mode[DISP_ID]->hdisplay, drm.mode[DISP_ID]->vdisplay);

	ret = initGBM();
	if (ret) 
	{
		printf("failed to initialize GBM\n");
		return ret;
	}

	ret = initEGL();
	if (ret) 
	{
		printf("failed to initialize EGL\n");
		return ret;
	}

	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(drm.fd, &fds);
	bo = gbm_surface_lock_front_buffer(gbm.surface);
	struct gbm_bo *fb = drm_fb_get_from_bo(bo);

	/* set mode: */
	int all_display = 0;
	if (all_display) {
		for (i = 0; i<drm.ndisp; i++) {
			ret = drmModeSetCrtc(drm.fd, drm.crtc_id[i], fb->fb_id, 0, 0,
				&drm.connector_id[i], 1, drm.mode[i]);
			if (ret) {
				printf("display %d failed to set mode: %s\n", i, strerror(errno));
				return ret;
			}
		}
	}
	else {
		ret = drmModeSetCrtc(drm.fd, drm.crtc_id[DISP_ID], fb->fb_id,
			0, 0, &drm.connector_id[DISP_ID], 1, drm.mode[DISP_ID]);
		if (ret) {
			printf("display %d failed to set mode: %s\n", DISP_ID, strerror(errno));
			return ret;
		}
	}
}

void EGL::deinit()
{
	deinitEGL();
	deinitGBM();
	deinitDRM();
}

void EGL::pushBuffer()
{
	eglSwapBuffers(gl.display, gl.surface);

	struct gbm_bo *next_bo = gbm_surface_lock_front_buffer(gbm.surface);
	struct drm_fb *fb = drm_fb_get_from_bo(next_bo);

	/*
	* Here you could also update drm plane layers if you want
	* hw composition
	*/
	int waiting_for_flip = 1;
	ret = drmModePageFlip(drm.fd, drm.crtc_id[DISP_ID], fb->fb_id, DRM_MODE_PAGE_FLIP_EVENT, &waiting_for_flip);
	if (ret) 
	{
		printf("failed to queue page flip: %s\n", strerror(errno));
		return;
	}

	while (waiting_for_flip) {
		ret = select(drm.fd + 1, &fds, NULL, NULL, NULL);
		if (ret < 0) {
			printf("select err: %s\n", strerror(errno));
			return;
		}
		else if (ret == 0) {
			printf("select timeout!\n");
			return;
		}
		else if (FD_ISSET(0, &fds)) {
			continue;
		}
		drmHandleEvent(drm.fd, &evctx);
	}

	/* release last buffer to render on again: */
	gbm_surface_release_buffer(gbm.surface, bo);
	bo = next_bo;
}

void EGL::initDRM()
{
	static const char *modules[] = {
		"omapdrm", "i915", "radeon", "nouveau", "vmwgfx", "exynos"
	};
	drmModeRes *resources;
	drmModeConnector *connector = NULL;
	drmModeEncoder *encoder = NULL;
	int i, j;
	uint32_t maxRes, curRes;

	for (i = 0; i < ARRAY_SIZE(modules); i++) {
		printf("trying to load module %s...", modules[i]);
		drm.fd = drmOpen(modules[i], NULL);
		if (drm.fd < 0) {
			printf("failed.\n");
		}
		else {
			printf("success.\n");
			break;
		}
	}

	if (drm.fd < 0) {
		printf("could not open drm device\n");
		return -1;
	}

	resources = drmModeGetResources(drm.fd);
	if (!resources) {
		printf("drmModeGetResources failed: %s\n", strerror(errno));
		return -1;
	}
	drm.resource_id = (uint32_t)resources;

	/* find a connected connector: */
	for (i = 0; i < resources->count_connectors; i++) {
		connector = drmModeGetConnector(drm.fd, resources->connectors[i]);
		if (connector->connection == DRM_MODE_CONNECTED) {
			/* choose the first supported mode */
			drm.mode[drm.ndisp] = &connector->modes[0];
			drm.connector_id[drm.ndisp] = connector->connector_id;

			for (j = 0; j<resources->count_encoders; j++) {
				encoder = drmModeGetEncoder(drm.fd, resources->encoders[j]);
				if (encoder->encoder_id == connector->encoder_id)
					break;

				drmModeFreeEncoder(encoder);
				encoder = NULL;
			}

			if (!encoder) {
				printf("no encoder!\n");
				return -1;
			}

			drm.encoder[drm.ndisp] = (uint32_t)encoder;
			drm.crtc_id[drm.ndisp] = encoder->crtc_id;
			drm.connectors[drm.ndisp] = connector;

			printf("### Display [%d]: CRTC = %d, Connector = %d\n", drm.ndisp, drm.crtc_id[drm.ndisp], drm.connector_id[drm.ndisp]);
			printf("\tMode chosen [%s] : Clock => %d, Vertical refresh => %d, Type => %d\n", drm.mode[drm.ndisp]->name, drm.mode[drm.ndisp]->clock, drm.mode[drm.ndisp]->vrefresh, drm.mode[drm.ndisp]->type);
			printf("\tHorizontal => %d, %d, %d, %d, %d\n", drm.mode[drm.ndisp]->hdisplay, drm.mode[drm.ndisp]->hsync_start, drm.mode[drm.ndisp]->hsync_end, drm.mode[drm.ndisp]->htotal, drm.mode[drm.ndisp]->hskew);
			printf("\tVertical => %d, %d, %d, %d, %d\n", drm.mode[drm.ndisp]->vdisplay, drm.mode[drm.ndisp]->vsync_start, drm.mode[drm.ndisp]->vsync_end, drm.mode[drm.ndisp]->vtotal, drm.mode[drm.ndisp]->vscan);

			/* If a connector_id is specified, use the corresponding display */
			if ((connector_id != -1) && (connector_id == drm.connector_id[drm.ndisp]))
				DISP_ID = drm.ndisp;

			/* If all displays are enabled, choose the connector with maximum
			* resolution as the primary display */
			if (all_display) {
				maxRes = drm.mode[DISP_ID]->vdisplay * drm.mode[DISP_ID]->hdisplay;
				curRes = drm.mode[drm.ndisp]->vdisplay * drm.mode[drm.ndisp]->hdisplay;

				if (curRes > maxRes)
					DISP_ID = drm.ndisp;
			}

			drm.ndisp++;
		}
		else {
			drmModeFreeConnector(connector);
		}
	}

	if (drm.ndisp == 0) {
		/* we could be fancy and listen for hotplug events and wait for
		* a connector..
		*/
		printf("no connected connector!\n");
		return -1;
	}

	return 0;
}

void EGL::initGBM()
{
	gbm.dev = gbm_create_device(drm.fd);

	gbm.surface = gbm_surface_create(gbm.dev,
		drm.mode[DISP_ID]->hdisplay, drm.mode[DISP_ID]->vdisplay,
		GBM_FORMAT_XRGB8888,
		GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
	if (!gbm.surface) {
		printf("failed to create gbm surface\n");
		return -1;
	}

	return 0;
}

void EGL::deinitDRM()
{
	drmModeRes *resources;
	int i;

	resources = (drmModeRes *)drm.resource_id;
	for (i = 0; i < resources->count_connectors; i++) {
		drmModeFreeEncoder((drmModeEncoderPtr)drm.encoder[i]);
		drmModeFreeConnector(drm.connectors[i]);
	}
	drmModeFreeResources((drmModeResPtr)drm.resource_id);
	drmClose(drm.fd);
	return;
}

void EGL::deinitGBM()
{
	gbm_surface_destroy(gbm.surface);
	gbm_device_destroy(gbm.dev);
	return;
}

//////////////////////

void EGL::initEGL()
{
	EGLint major, minor, n;

	static const EGLint context_attribs[] = 
	{
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	static const EGLint config_attribs[] = 
	{
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_RED_SIZE, 1,
		EGL_GREEN_SIZE, 1,
		EGL_BLUE_SIZE, 1,
		EGL_ALPHA_SIZE, 0,
		EGL_SAMPLES, 4,
		EGL_NONE
	};

	gl.display = eglGetDisplay((EGLNativeDisplayType)gbm.dev);

	if (!eglInitialize(gl.display, &major, &minor)) 
	{
		printf("failed to initialize\n");
		return -1;
	}

	printf("Using display %p with EGL version %d.%d\n",
		gl.display, major, minor);

	printf("EGL Version \"%s\"\n", eglQueryString(gl.display, EGL_VERSION));
	printf("EGL Vendor \"%s\"\n", eglQueryString(gl.display, EGL_VENDOR));
	printf("EGL Extensions \"%s\"\n", eglQueryString(gl.display, EGL_EXTENSIONS));

	if (!eglBindAPI(EGL_OPENGL_ES_API)) {
		printf("failed to bind api EGL_OPENGL_ES_API\n");
		return -1;
	}

	if (!eglChooseConfig(gl.display, config_attribs, &gl.config, 1, &n) || n != 1) {
		printf("failed to choose config: %d\n", n);
		return -1;
	}

	gl.context = eglCreateContext(gl.display, gl.config,
		EGL_NO_CONTEXT, context_attribs);
	if (gl.context == NULL) {
		printf("failed to create context\n");
		return -1;
	}

	gl.surface = eglCreateWindowSurface(gl.display, gl.config, gbm.surface, NULL);
	if (gl.surface == EGL_NO_SURFACE) {
		printf("failed to create egl surface\n");
		return -1;
	}

	/* connect the context to the surface */
	eglMakeCurrent(gl.display, gl.surface, gl.surface, gl.context);

	return 0;
}

void EGL::deinitEGL()
{
	eglDestroySurface(gl.display, gl.surface);
	eglDestroyContext(gl.display, gl.context);
	eglTerminate(gl.display);
}

#endif
