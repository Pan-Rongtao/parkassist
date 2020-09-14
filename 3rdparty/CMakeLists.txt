
# 0. CMake Defs

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

cmake_policy(VERSION 2.6)

# 1. Project Name

project(3rdparty)

# 2. Env (Include/Lib Path, C/CXX/LD FLAGS)


# 4. Sub Projects

# 5. Project Install


if(WIN32)
	file(GLOB LIBS win32/*.dll)
elseif(UNIX AND NOT ANDROID)
	file(GLOB LIBS linux-arm/*.so*)
endif()
install(FILES ${LIBS} DESTINATION ${CMAKE_INSTALL_LIBDIR})

