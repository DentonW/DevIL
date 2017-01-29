add_executable(benchmark EXCLUDE_FROM_ALL benchmark.c)
target_link_libraries(benchmark IL ILU)
target_include_directories(benchmark PRIVATE ${DevIL_SOURCE_DIR}/../include)

