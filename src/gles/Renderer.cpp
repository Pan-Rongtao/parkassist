#include "parkassist/gles/Renderer.h"
#include <GLES2/gl2.h>
#include "glm/glm.hpp"
#include "parkassist/gles/Mesh.h"
#include "parkassist/gles/Camera.h"

using namespace nb;

Renderer::Renderer()
	: Renderer(nullptr, nullptr)
{
}

Renderer::Renderer(MeshPtr mesh)
	: Renderer(mesh, nullptr)
{
}

Renderer::Renderer(MeshPtr mesh, ProgramPtr program)
	: m_mesh(mesh)
	, m_program(program)
{
}

void Renderer::setMesh(MeshPtr mesh)
{
	m_mesh = mesh;
}

MeshPtr Renderer::mesh()
{
	return m_mesh;
}

void Renderer::setProgram(ProgramPtr program)
{
	m_program = program;
}

ProgramPtr Renderer::program()
{
	return m_program;
}

void Renderer::draw(CameraPtr camera) const
{
	if (!m_mesh || !m_program)
		return;

	auto &program = m_program;
	program->use();

	//计算后的mvp，以及分开的m/v/p
	auto const &m = m_mesh->matrix;
	auto const &v = camera->viewMatrix();
	auto const &p = camera->projectionMatrix();
	auto mvp = p * v * m;
	program->uniform(program->getUniformLocation(Program::nbMvpStr), mvp);
	program->uniform(program->getUniformLocation(Program::nbMStr), m);
	program->uniform(program->getUniformLocation(Program::nbVStr), v);
	program->uniform(program->getUniformLocation(Program::nbPStr), p);

	//storage中的uniform
	for (auto const &iter : m_uniforms)
	{
		auto location = program->getUniformLocation(iter.first.data());
		auto const &v = iter.second;
		if (v.is<int>())							program->uniform(location, any_cast<int>(v));
		else if (v.is<unsigned int>())				program->uniform(location, (int)any_cast<unsigned int>(v));
		else if (v.is<short>())						program->uniform(location, (int)any_cast<short>(v));
		else if (v.is<unsigned short>())			program->uniform(location, (int)any_cast<unsigned short>(v));
		else if (v.is<long>())						program->uniform(location, (int)any_cast<long>(v));
		else if (v.is<unsigned long>())				program->uniform(location, (int)any_cast<unsigned long>(v));
		else if (v.is<float>())						program->uniform(location, any_cast<float>(v));
		else if (v.is<double>())					program->uniform(location, (float)any_cast<double>(v));
		else if (v.is<glm::vec2>())					program->uniform(location, any_cast<glm::vec2>(v));
		else if (v.is<glm::vec3>())					program->uniform(location, any_cast<glm::vec3>(v));
		else if (v.is<glm::vec4>())					program->uniform(location, any_cast<glm::vec4>(v));
		else if (v.is<glm::mat2x2>())				program->uniform(location, any_cast<glm::mat2x2>(v));
		else if (v.is<glm::mat3x3>())				program->uniform(location, any_cast<glm::mat3x3>(v));
		else if (v.is<glm::mat4x4>())				program->uniform(location, any_cast<glm::mat4x4>(v));
		else if (v.is<glm::ivec2>())				program->uniform(location, any_cast<glm::ivec2>(v));
		else if (v.is<glm::ivec3>())				program->uniform(location, any_cast<glm::ivec3>(v));
		else if (v.is<glm::ivec4>())				program->uniform(location, any_cast<glm::ivec4>(v));
		else if (v.is<std::vector<int>>())			program->uniform(location, any_cast<std::vector<int>>(v));
		else if (v.is<std::vector<float>>())		program->uniform(location, any_cast<std::vector<float>>(v));
		else if (v.is<std::vector<glm::vec2>>())	program->uniform(location, any_cast<std::vector<glm::vec2>>(v));
		else if (v.is<std::vector<glm::vec3>>())	program->uniform(location, any_cast<std::vector<glm::vec3>>(v));
		else if (v.is<std::vector<glm::vec4>>())	program->uniform(location, any_cast<std::vector<glm::vec4>>(v));
		else if (v.is<std::vector<glm::mat2x2>>())	program->uniform(location, any_cast<std::vector<glm::mat2x2>>(v));
		else if (v.is<std::vector<glm::mat3x3>>())	program->uniform(location, any_cast<std::vector<glm::mat3x3>>(v));
		else if (v.is<std::vector<glm::mat4x4>>())	program->uniform(location, any_cast<std::vector<glm::mat4x4>>(v));
		else if (v.is<std::vector<glm::ivec2>>())	program->uniform(location, any_cast<std::vector<glm::ivec2>>(v));
		else if (v.is<std::vector<glm::ivec3>>())	program->uniform(location, any_cast<std::vector<glm::ivec3>>(v));
		else if (v.is<std::vector<glm::ivec4>>())	program->uniform(location, any_cast<std::vector<glm::ivec4>>(v));
		else										printf("%s is not a supported type for glsl uniform.\n", v.type().name());
	}

	program->vertexAttributePointer(Program::nbPositionLocation, Vertex::positionDimension, Vertex::stride, m_mesh->positionData());
	program->vertexAttributePointer(Program::nbColorLocation, Vertex::colorDimension, Vertex::stride, m_mesh->colorData());
	program->vertexAttributePointer(Program::nbNormalLocation, Vertex::normalDimension, Vertex::stride, m_mesh->normalData());

	program->vertexAttributePointer(Program::nbTexCoordLocaltion, Vertex::texCoordDimension, Vertex::stride, m_mesh->textureCoordinateData());
	for (auto const &tex : m_mesh->material.textures())
	{
		tex->bind();
		tex->active();
	}

	glDrawElements(m_mesh->mode, m_mesh->indices.size(), GL_UNSIGNED_SHORT, m_mesh->indices.data());
	program->disuse();
}
