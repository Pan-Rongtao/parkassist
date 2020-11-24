
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

# 1. Project Name

project(parkassist)

# 2. Project Env (Include/Lib Path, C/CXX/LD FLAGS)

# 3. Project Build

set(LIB_NAME "parkassist")

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/include/parkassist/*.h" "*.h" "*.cpp" "*.c" "${CMAKE_SOURCE_DIR}/include/*.h")

add_library(${LIB_NAME} SHARED ${SRC_FILES})
set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME parkassist)
if(WIN32 AND NOT MINGW)
	target_link_libraries(${LIB_NAME} glfw3)
elseif(UNIX AND NOT ANDROID)
	target_link_libraries(${LIB_NAME} GLESv2)
endif()

# 4. Project Install
install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_INSTALL_LIBDIR})
