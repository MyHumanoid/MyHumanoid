cmake_minimum_required(VERSION 3.5)

project(MyHuman LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(animorph-0.3)
#add_subdirectory(mhgui-0.2)
add_subdirectory(makehuman-0.9.1-rc1a)
