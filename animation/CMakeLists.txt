cmake_minimum_required(VERSION 3.4)

set(project_name "animation")
# setting project name
project(${project_name})

# here add all the cpp files involved in the project
add_executable(${project_name} ./include/modelloader.cpp 
							   ./include/shader.cpp 
							   ./include/gamesettings.cpp 
							   ./src/main.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_BUILD_TYPE Debug) #required for ycm
set(CMAKE_VERBOSE_MAKEFILE ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# add all the include files
target_include_directories(${project_name} PUBLIC ./include PUBLIC /usr/include PUBLIC /usr/local/include) # you can keep adding more if you want

#here add all the libraries that you included
target_link_libraries(${project_name} glfw3 GLEW GL GLU SOIL dl Xinerama Xrandr Xi Xcursor X11 Xxf86vm pthread assimp) # this is and example

# Required for YCM
IF(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
 EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
	${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
	)
ENDIF()
