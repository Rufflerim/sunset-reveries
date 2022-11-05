file( GLOB GAME_SOURCES *.cpp )
add_library( game ${GAME_SOURCES})
target_include_directories(game PUBLIC gaemi)

target_link_libraries(game raylib)