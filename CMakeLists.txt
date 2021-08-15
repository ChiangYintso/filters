cmake_minimum_required(VERSION 3.16)
project(filters)

set(CMAKE_C_STANDARD 11)

add_compile_options(-mavx2)
add_library(blocked_bloom_filter STATIC src/blocked_bloom_filter.c)
target_include_directories(blocked_bloom_filter PUBLIC include)

add_library(bloom_filter STATIC src/bloom_filter.c)
target_include_directories(bloom_filter PUBLIC include)

option(FILTERS_TEST "Build filters test" OFF)
if (FILTERS_TEST)
	add_definitions(-DFILTERS_TEST)
	add_definitions(-DFILTERS_TEST_MAIN)
	message(STATUS "Build filters test")
	
	add_executable(blocked_bloom_filter_test src/blocked_bloom_filter.c)
	target_include_directories(blocked_bloom_filter_test PUBLIC include)

	add_executable(bloom_filter_test src/bloom_filter.c)
	target_include_directories(bloom_filter_test PUBLIC include)
endif(FILTERS_TEST)

option(FILTERS_BENCH "Build filters benchmark" OFF)
if (FILTERS_BENCH)
	message(STATUS "Build filters benchmark")

	add_executable(blocked_bloom_filter_bench ${PROJECT_SOURCE_DIR}/benchmark/blocked_bloom_filter_bench.c)
	target_link_libraries(blocked_bloom_filter_bench blocked_bloom_filter)
	target_include_directories(blocked_bloom_filter_bench PUBLIC include)

	add_executable(bloom_filter_bench ${PROJECT_SOURCE_DIR}/benchmark/bloom_filter_bench.c)
	target_link_libraries(bloom_filter_bench bloom_filter)
	target_include_directories(bloom_filter_bench PUBLIC include)
endif(FILTERS_BENCHMARK)
