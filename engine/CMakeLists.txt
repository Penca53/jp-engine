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

add_library(engine app.cc camera.cc camera_manager.cc circle_collider.cc collider.cc input.cc node.cc physics.cc rectangle_collider.cc resource_manager.cc scene.cc sprite_sheet_animation.cc tile.cc tilemap.cc tileset.cc)
target_compile_features(engine PRIVATE cxx_std_23)
set_target_properties(engine PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(engine PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

target_link_libraries(engine PUBLIC SFML::Audio SFML::Graphics)
target_include_directories(engine PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/..")

target_compile_definitions(engine PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")