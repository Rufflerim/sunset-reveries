cmake_minimum_required(VERSION 3.20)
project(gecs)
set(CMAKE_CXX_STANDARD 20)

file( GLOB GECS_SOURCES *.cpp )
add_library( gecs ${GECS_SOURCES})
target_include_directories(gecs PUBLIC gaemi)

target_link_libraries(gecs gaemi gmath)