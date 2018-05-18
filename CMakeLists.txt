cmake_minimum_required(VERSION 3.10)
project(openverse VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Common
add_subdirectory(common)

target_include_directories(common_test PRIVATE extern/doctest/doctest)

# Server
add_subdirectory(server)
target_include_directories(server PRIVATE common/include)

target_include_directories(server_test PRIVATE extern/doctest/doctest)

# Client
add_subdirectory(client)
target_include_directories(client PRIVATE common/include)

target_include_directories(client_test PRIVATE extern/doctest/doctest)

