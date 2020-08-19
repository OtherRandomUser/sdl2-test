cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
project(SDL2-Test)

add_executable(sdl2-test main.cpp)

find_package(SDL2 REQUIRED)

target_include_directories(sdl2-test
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(sdl2-test ${SDL2_LIBRARIES})
