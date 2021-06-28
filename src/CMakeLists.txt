cmake_minimum_required(VERSION "3.18")
project("cgcs_list")

set(C_STANDARD "11") # C11
set(CFLAGS "-Wall -Werror -pedantic-errors")

set(CMAKE_C_STANDARD ${C_STANDARD})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${CFLAGS})

add_library("cgcs_list" "cgcs_list.h" "cgcs_list.c")
target_compile_options("cgcs_list" PUBLIC "-fblocks")
target_include_directories("cgcs_list" PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
