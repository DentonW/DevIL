cmake_minimum_required(VERSION 2.6)

project(ImageLib)
# include our custom modules
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

add_subdirectory(src-IL)
add_subdirectory(src-ILU)
add_subdirectory(src-ILUT)

