file(GLOB CPP_FILES src/*.cpp)
file(GLOB H_FILES src/*.h include/*.h)
set(SOURCE_FILES ${CPP_FILES} ${H_FILES})
add_library(server STATIC ${SOURCE_FILES})

file(GLOB TEST_CPP_FILES test/*.cpp)
file(GLOB TEST_H_FILES test/*.h)
set(TEST_FILES ${TEST_CPP_FILES} ${TEST_H_FILES})
add_executable(server_test ${TEST_FILES})
