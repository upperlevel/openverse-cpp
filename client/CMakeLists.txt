find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

set(SOURCE_FILES src/main.cpp)
add_executable(client ${SOURCE_FILES})
target_link_libraries(client common server ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} glfw)

set(TEST_FILES test/main.cpp)
add_executable(client_test ${TEST_FILES})
target_include_directories(client_test PUBLIC client)
