cmake_minimum_required(VERSION 3.10)
project(openverse)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL)
find_package(GLEW)
find_package(glfw3)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

set(SOURCE_FILES src/launcher/Main.cpp src/common/Chunk.h src/common/Chunk.cpp src/common/Block.cpp src/common/Block.h)

add_executable(openverse ${SOURCE_FILES})
target_link_libraries(openverse ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} glfw)
