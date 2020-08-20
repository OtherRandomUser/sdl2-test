cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)
project(SDL2-Test)

find_package(SDL2 REQUIRED)

# points
add_executable(points points.cpp)

target_include_directories(points
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(points ${SDL2_LIBRARIES})

# random points
add_executable(random_points random_points.cpp)

target_include_directories(random_points
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(random_points ${SDL2_LIBRARIES})

# lines
add_executable(lines lines.cpp)

target_include_directories(lines
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(lines ${SDL2_LIBRARIES})
