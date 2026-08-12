cmake_minimum_required(VERSION 3.10)
project(habibito)

set(SOURCES
    main.c
    data_manager.c
    handler.c
    painter.c
    utils.c
)

add_executable(habibito ${SOURCES})
