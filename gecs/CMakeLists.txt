cmake_minimum_required(VERSION 3.20)
project(gecs)
set(CMAKE_CXX_STANDARD 20)

file( GLOB GECS_SOURCES *.cpp )
add_library( gecs ${GECS_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gecs gplatform gmath glog)