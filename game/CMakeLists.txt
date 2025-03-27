cmake_minimum_required(VERSION 3.28)
project(game LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

add_executable(game main.cc background.cc banana.cc default_scene.cc end.cc follow_player.cc game_manager.cc goomba.cc lose_canvas.cc mario.cc plant.cc plant_bullet.cc score_manager.cc win_canvas.cc)
target_compile_features(game PRIVATE cxx_std_23)
target_link_libraries(game PRIVATE engine)

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/copy-assets.cmake
)
add_dependencies(game copy_assets)

add_compile_options(-fsanitize=address,undefined)
add_link_options(-fsanitize=address,undefined)

