set(SOURCE_FILES include/server.h)
add_library(server STATIC ${SOURCE_FILES})

set(TEST_FILES test/main.cpp)
add_executable(server_test ${TEST_FILES})
