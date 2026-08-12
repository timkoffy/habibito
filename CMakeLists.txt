cmake_minimum_required(VERSION 3.10)
project(habibito)

add_subdirectory(src)

add_custom_target(run
    COMMAND $<TARGET_FILE:habibito>
    DEPENDS habibito
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
