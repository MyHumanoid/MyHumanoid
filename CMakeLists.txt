cmake_minimum_required(VERSION 3.10)

project(MyHumanoid LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern)
add_subdirectory(animorph)
add_subdirectory(makehuman)
