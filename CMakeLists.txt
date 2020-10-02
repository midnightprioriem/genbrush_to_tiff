
cmake_minimum_required( VERSION 2.8 )

project(genbrush_to_tiff)

find_package(TIFF)

add_executable(genbrush_to_tiff main.cpp)

target_link_libraries(genbrush_to_tiff TIFF::TIFF)
