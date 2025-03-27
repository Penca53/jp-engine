cmake_minimum_required(VERSION 3.28)
project(engine LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(engine app.cc camera.cc circle_collider.cc collider.cc fsm.cc input.cc node.cc physics.cc rectangle_collider.cc resource_manager.cc sprite_sheet_animation.cc state.cc tile.cc tilemap.cc tileset.cc transition.cc)
target_compile_features(engine PRIVATE cxx_std_23)
set_target_properties(engine PROPERTIES CXX_EXTENSIONS OFF)

if (MSVC)
    target_compile_options(engine PRIVATE "/std:c++latest")
endif()

target_link_libraries(engine PUBLIC SFML::Audio SFML::Graphics)
target_include_directories(engine PUBLIC ${CMAKE_SOURCE_DIR})