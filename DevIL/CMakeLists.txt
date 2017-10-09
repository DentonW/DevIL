cmake_minimum_required(VERSION 2.6)

project(ImageLib)
# include our custom modules
set (CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

add_subdirectory(src-IL)

option(IL_BUILD_ILU "build ILU library" ON)
if (IL_BUILD_ILU)
    add_subdirectory(src-ILU)
endif()

option(IL_BUILD_ILUT "build ILUT library" ON)
#Could avoid this extra check with CMakeDependentOption
#But that would require bumping cmake version to 3.1
if (IL_BUILD_ILUT AND (NOT IL_BUILD_ILU))
    message(FATAL_ERROR "Can't build ILUT without ILU. Enable IL_BUILD_ILU")
elseif(IL_BUILD_ILUT)
    add_subdirectory(src-ILUT)
endif()

option(IL_TESTS "build DevIL tests" ON)
if (IL_TESTS)
    ENABLE_TESTING()
    Add_Subdirectory(test)
endif()

