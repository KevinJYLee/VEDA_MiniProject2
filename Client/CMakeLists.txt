
cmake_minimum_required(VERSION 3.0)
project(MiniProject2_Client)

include_directories(include)

set(SOURCES
    src/myClient.c
)

add_executable(MiniProject2_Client ${SOURCES})
