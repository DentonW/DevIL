cmake_minimum_required(VERSION 2.6)

project(ImageLib)
# include our custom modules
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

add_subdirectory(src-IL)
add_subdirectory(src-ILU)
add_subdirectory(src-ILUT)
option(IL_TESTS "build DevIL tests" ON)
if (IL_TESTS)
    ENABLE_TESTING()
    Add_Subdirectory(test)
endif()

