cmake_minimum_required(VERSION 3.30.0)

project(ppos)

add_executable(${PROJECT_NAME}_1 pingpong-tasks1.c ppos_core.c)
add_executable(${PROJECT_NAME}_2 pingpong-tasks2.c ppos_core.c)
add_executable(${PROJECT_NAME}_3 pingpong-tasks3.c ppos_core.c)
