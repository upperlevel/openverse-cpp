set(SOURCE_FILES include/block.h src/world.cpp include/world.h include/util.h src/util.cpp include/event.h)
add_library(common STATIC ${SOURCE_FILES})

set(TEST_FILES test/main.cpp test/util.h test/event.h)
add_executable(common_test ${TEST_FILES})
target_link_libraries(common_test common)
