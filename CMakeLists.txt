cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)

project(string_synchronizing_set)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add TLX command line parser
add_library(tlx
  extlib/tlx/tlx/cmdline_parser.cpp
  extlib/tlx/tlx/string/parse_si_iec_units.cpp
  extlib/tlx/tlx/die/core.cpp)

target_include_directories(tlx PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extlib/tlx/>
  $<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/extlib/tlx/>
)

# Add malloc_count
add_library(malloc_count OBJECT
  ${PROJECT_SOURCE_DIR}/extlib/malloc_count/malloc_count.c
  ${PROJECT_SOURCE_DIR}/extlib/malloc_count/stack_count.c
) 
target_include_directories(malloc_count PUBLIC
  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/extlib/malloc_count/>
  $<INSTALL_INTERFACE:${PROJECT_SOURCE_DIR}/extlib/malloc_count/>
)

add_subdirectory(string_synchronizing_set)
add_subdirectory(benchmark)
add_subdirectory(tests)
