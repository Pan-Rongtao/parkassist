
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(tests)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

if(NOT WIN32)
	include_directories(/work/sdk/sv-ab02a-system-sdk/sysroots/armv7ahf-neon-linux-gnueabi/usr/include/libdrm)
endif()

# 3. Project Build

set(BIN_NAME "tests")

file(GLOB SRC_FILES "*.h" "*.cpp" "../*.h" "../*.cpp")

add_executable(${BIN_NAME} ${SRC_FILES})
set_target_properties(${BIN_NAME} PROPERTIES OUTPUT_NAME nb.test.${BIN_NAME})
if(WIN32 AND NOT MINGW)
	target_link_libraries(${BIN_NAME} gles glfw3)
else()
	target_link_libraries(${BIN_NAME} gles EGL drm gbm)
endif()

# 4. Project Install
install(TARGETS ${BIN_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
