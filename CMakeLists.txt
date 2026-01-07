cmake_minimum_required(VERSION 3.20)
project(ITCH LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DNDEBUG -O3 -march=native -g")
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)

find_package(absl REQUIRED)

add_library(itch_parser INTERFACE)
target_include_directories(itch_parser INTERFACE include)
target_link_libraries(itch_parser INTERFACE absl::flat_hash_map)

add_executable(benchmark src/main.cpp)
target_link_libraries(benchmark PRIVATE itch_parser)
