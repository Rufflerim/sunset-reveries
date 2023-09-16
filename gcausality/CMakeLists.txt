cmake_minimum_required(VERSION 3.20)
project(gcausality)
set(CMAKE_CXX_STANDARD 20)

file( GLOB GCAUSALITY_SOURCES *.cpp )
add_library( gcausality ${GCAUSALITY_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(gcausality gecs gmath gplatform)