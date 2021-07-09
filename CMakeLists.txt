
# You are free to modify this file

cmake_minimum_required(VERSION 3.0)
project(game)
find_package(X11 REQUIRED)
set(CMAKE_CONFIGURATION_TYPES "Debug" "Release")
file(GLOB SRC *.cpp)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/data/background.png
                ${CMAKE_CURRENT_BINARY_DIR}/background.png
                COPYONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/data/background2.png
                ${CMAKE_CURRENT_BINARY_DIR}/background2.png
                COPYONLY)
configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/data/bird.png
                ${CMAKE_CURRENT_BINARY_DIR}/bird.png
                COPYONLY)
add_executable(game ${SRC})
target_link_libraries(game m X11)
