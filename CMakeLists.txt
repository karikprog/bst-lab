cmake_minimum_required(VERSION 3.25)
project(bst C)

add_library(Bst src/bst.c)

add_executable(bstProgram src/main.c)
target_link_libraries(bstProgram PRIVATE Bst)