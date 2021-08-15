cmake_minimum_required(VERSION 3.16)
project(filters)

set(CMAKE_C_STANDARD 11)

add_compile_options(-mavx2)
add_library(blocked_bloom_filter STATIC src/blocked_bloom_filter.c)
target_include_directories(blocked_bloom_filter PUBLIC include)

option(FILTERS_TEST "Build filters test" OFF)
if (FILTERS_TEST)
	add_definitions(-DFILTERS_TEST)
	add_definitions(-DFILTERS_TEST_MAIN)
	message(STATUS "Build filters test")
	add_executable(blocked_bloom_filter_test src/blocked_bloom_filter.c)
	target_include_directories(blocked_bloom_filter_test PUBLIC include)
endif(FILTERS_TEST)
