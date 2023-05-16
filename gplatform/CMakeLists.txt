cmake_minimum_required(VERSION 3.20)
project(gplatform)
set(CMAKE_CXX_STANDARD 20)

if (WIN32)
    file( GLOB platform_SOURCES win/*.cpp )
elseif (UNIX)
    file( GLOB platform_SOURCES linux/*.cpp )
endif ()

add_library( ${PROJECT_NAME} ${platform_SOURCES} )
target_include_directories( ${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} )
#target_link_libraries( ${PROJECT_NAME} raylib )

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(gplatform PUBLIC DEBUG=1)
else()
    target_compile_definitions(gplatform PUBLIC DEBUG=0)
endif()

