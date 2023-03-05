cmake_minimum_required(VERSION 3.20)
project(gaemi)
set(CMAKE_CXX_STANDARD 20)

file( GLOB gaemi_SOURCES *.cpp )
add_library( ${PROJECT_NAME} SHARED ${gaemi_SOURCES} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
target_link_libraries( ${PROJECT_NAME} raylib )

if (WIN32)
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/gaemi.dll ${CMAKE_CURRENT_BINARY_DIR}/../
            COMMENT "-- Copying Engine DLL"
    )
endif(WIN32)