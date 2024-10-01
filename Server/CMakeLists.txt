cmake_minimum_required(VERSION 3.0)
project(MiniProject2_Server)

include_directories(include)

set(SOURCES
    src/myServer.c
)

add_executable(MiniProject2_Server ${SOURCES})
