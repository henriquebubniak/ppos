cmake_minimum_required(VERSION 3.30.0)

project(ppos)

add_executable(${PROJECT_NAME} pingpong-dispatcher.c ppos_core.c queue.c)
