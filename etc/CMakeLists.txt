# 1. Project Name

project(etc)

# 2. Platform Env (Include/Lib Path, C/CXX/LD FLAGS)



# 3. Sub Projects


# 5. Project Install
file(GLOB PICS *.jpg* *.png* *.bmp*)
install(FILES ${PICS} DESTINATION ${CMAKE_INSTALL_ETCDIR})

