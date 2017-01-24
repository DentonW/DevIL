
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(cppunit)
add_executable(UnitTest EXCLUDE_FROM_ALL ILTest.cpp ILUTest.cpp UnitTest.cpp)
target_include_directories(UnitTest PRIVATE ${cppunit_INCLUDE_DIRECTORIES})
target_link_libraries(UnitTest IL ILU ${CPPUNIT_LIBRARIES})
