cmake_minimum_required(VERSION 3.25)
project(bst C)

enable_testing()
add_library(Bst src/bst.c)
add_library(Tests src/tests.c)

add_executable(app src/main.c)
target_link_libraries(app PRIVATE Bst)
target_link_libraries(app PRIVATE Tests)

add_test(
    NAME BstTests
    COMMAND app --test
)