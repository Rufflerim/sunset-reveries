#[===[
Here is the architecture of the gaemi engine.

---------------------------------
|             GAME              |
---------------------------------
|         gaemi (engine)        |
----------------------------------------------
|    gplatform    |    gmath    |    LIBS    |
----------------------------------------------

]===]

cmake_minimum_required(VERSION 3.20)
project(sunset-reveries)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#if (UNIX)
#    option(BUILD_SHARED_LIBS "Build shared libraries (.dll/.so) instead of static ones (.lib/.a)" ON)
#endif(UNIX)

# Dependencies
## -- OpenGL
if (UNIX)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif(UNIX)
cmake_policy(SET CMP0135 NEW)
## -- Raylib
set(RAYLIB_VERSION 5.0)
find_package(raylib ${RAYLIB_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    if (UNIX)
        FetchContent_Declare(
                raylib
                URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
        )
    elseif (WIN32)
        FetchContent_Declare(
                raylib
                URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.zip
        )
    endif()
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
endif()

## -- Raylib GUI
set(RAYLIB_GUI_VERSION 4.0)
find_package(raylib_gui ${RAYLIB_GUI_VERSION} QUIET) # QUIET or REQUIRED
if (NOT raylib_gui_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)
    if (UNIX)
        FetchContent_Declare(
                raylib_gui
                URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYLIB_GUI_VERSION}.tar.gz
        )
    elseif (WIN32)
        FetchContent_Declare(
                raylib_gui
                URL https://github.com/raysan5/raygui/archive/refs/tags/${RAYLIB_GUI_VERSION}.zip
        )
    endif()
    FetchContent_GetProperties(raylib_gui)
    if (NOT raylib_gui_POPULATED) # Have we downloaded raylib gui yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib_gui)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
        file(COPY ${raylib_gui_SOURCE_DIR}/projects/CMake/CMakeLists.txt
                DESTINATION ${raylib_gui_SOURCE_DIR})
        add_subdirectory(${raylib_gui_SOURCE_DIR} ${raylib_gui_BINARY_DIR})
        file(COPY ${raylib_gui_SOURCE_DIR}/src/raygui.h
                DESTINATION ${CMAKE_SOURCE_DIR}/gaemi/_Libs/raygui)
        file(COPY ${raylib_gui_SOURCE_DIR}/icons/raygui_icons.h
                DESTINATION ${CMAKE_SOURCE_DIR}/gaemi/_Libs/raygui/icons)
    endif()
endif()



# Add code.
add_subdirectory( gplatform )
add_subdirectory( glog )
add_subdirectory( gfile )
add_subdirectory( gmath )
add_subdirectory( gecs )
#add_subdirectory( gcausality )
add_subdirectory( gaemi )
add_subdirectory( sunset )



# Debug on Linux
if (UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
endif(UNIX)

# Declaring our executable
add_executable(${PROJECT_NAME} sunset/main.cpp)
target_link_libraries(${PROJECT_NAME} game gaemi)

# C++ hardcode options
if (WIN32)
    target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
endif(WIN32)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wno-c++98-compat -Wno-c++98-compat-pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wall)
endif()

# Copying assets to the build folder
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
        COMMENT "---- Copy Assets"
)

# Web Configurations
#if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
#    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
#endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()