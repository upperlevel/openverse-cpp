#
# Global include:
# file(GLOB CPP_FILES src/*.cpp)
# file(GLOB H_FILES src/*.h include/*.h)
#
# NOTE:
# Need to write files manually since CLion doesn't support global includes.
#

set(H_FILES
        include/block.h
        include/block_state.h
        include/chunk.h
        include/event.h
        include/item.h
        include/location.h
        include/network.h
        include/protocol.h
        include/util.h
        include/world.h
        include/chunk_pillar.h)

set(CPP_FILES
        src/block_state.cpp
        src/chunk.cpp
        src/item.cpp
        src/network.cpp
        src/protocol.cpp
        src/util.cpp
        src/world.cpp
        src/chunk_pillar.cpp)

set(SOURCE_FILES ${H_FILES} ${CPP_FILES})
add_library(common STATIC ${SOURCE_FILES})

file(GLOB TEST_CPP_FILES test/*.cpp)
file(GLOB TEST_H_FILES test/*.h)
set(TEST_FILES ${TEST_CPP_FILES} ${TEST_H_FILES})
add_executable(common_test ${TEST_FILES})
target_link_libraries(common_test common)
