cmake_minimum_required(VERSION "3.18")
project("cgcs_list_demo")

set(C_STANDARD "11")
set(CFLAGS "-Wall -Werror -pedantic-errors")

set(CMAKE_C_STANDARD ${C_STANDARD})
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} ${CFLAGS})

add_executable("cgcs_list_demo" "cgcs_list_demo.c")
target_compile_options("cgcs_list_demo" PUBLIC "-fblocks")
target_link_libraries("cgcs_list_demo" LINK_PUBLIC "cgcs_list")
