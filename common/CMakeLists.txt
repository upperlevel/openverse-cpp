set(SOURCE_FILES src/block.cpp include/block.h src/world.cpp include/world.h include/util.h src/util.cpp)
add_library(common STATIC ${SOURCE_FILES})

set(TEST_FILES test/main.cpp test/world.h test/block.h test/util.h)
add_executable(common_test ${TEST_FILES})
target_link_libraries(common_test common)
