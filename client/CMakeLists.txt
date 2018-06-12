# In order to successfully pass the dependency fetch:
# * Utilities:
# - FindThreads.cmake in $CMAKE_MODULE_PATH.
#
# * Graphics:
# - FindOpenGL.cmake in $CMAKE_MODULE_PATH.
# - FindGLEW.cmake in $CMAKE_MODULE_PATH.
# - glfw3/glfw3Config.cmake in cmake installation dir.
#   If you're on arch linux, you can simply download via pacman the 'glfw-x11' packet.

find_package(Threads REQUIRED)
if (NOT Threads_FOUND)
    message(FATAL_ERROR "Threads not found")
endif ()

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(FATAL_ERROR "OpenGL not found")
endif ()

find_package(GLEW REQUIRED)
if (NOT GLEW_FOUND)
    message(FATAL_ERROR "GLEW not found")
endif ()

find_package(glfw3 REQUIRED CONFIG)
if (NOT glfw3_FOUND)
    message(FATAL_ERROR "GLEW not found")
endif ()

file(GLOB CPP_FILES src/*.cpp)
file(GLOB H_FILES src/*.h include/*.h)
set(SOURCE_FILES ${CPP_FILES} ${H_FILES})
add_executable(client ${SOURCE_FILES})
target_include_directories(client PRIVATE ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
target_link_libraries(client common server ${CMAKE_THREAD_LIBS_INIT} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} glfw)

file(GLOB TEST_CPP_FILES test/*.cpp)
file(GLOB TEST_H_FILES test/*.h)
set(TEST_FILES ${TEST_CPP_FILES} ${TEST_H_FILES})
add_executable(client_test ${TEST_FILES})
target_include_directories(client_test PUBLIC client)
