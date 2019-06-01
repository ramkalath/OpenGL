cmake_minimum_required(VERSION 3.4)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

# set the project name properly for this
set(project_name "assimp_0") # (TODO: add project name)

# setting project name
project(${project_name})

# here add all the cpp files involved in the project
add_executable(${project_name} ./include/gamesettings.cpp ./include/modelloader.cpp ./include/shader.cpp ./src/main.cpp) # (TODO: enter the cpp files after the project name one after the other with the path)

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_VERBOSE_MAKEFILE ON)
set(EXECUTABLE_OUTPUT_PATH ../bin)

# add all the include files
target_include_directories(${project_name} PUBLIC ./include
								PUBLIC /usr/include 
								PUBLIC /usr/local/include) # you can keep adding more if you want

# here add all the libraries that you included
target_link_libraries(${project_name} glfw3 GLEW GL GLU SOIL dl Xinerama Xrandr Xi Xcursor X11 Xxf86vm pthread assimp) # this is and example

