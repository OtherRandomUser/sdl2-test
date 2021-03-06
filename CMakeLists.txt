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

# circles
add_executable(circles circles.cpp)

target_include_directories(circles
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(circles ${SDL2_LIBRARIES})

# bezier
add_executable(bezier bezier.cpp)

target_include_directories(bezier
    PUBLIC
        ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(bezier ${SDL2_LIBRARIES})
