add_executable(game main.cc background.cc banana.cc default_scene.cc end.cc follow_player.cc game_manager.cc lose_canvas.cc mushroom.cc plant.cc plant_bullet.cc player.cc score_manager.cc win_canvas.cc)

target_compile_features(game PRIVATE cxx_std_23)
set_target_properties(game PROPERTIES CXX_EXTENSIONS OFF)

target_compile_options(game PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

if (NOT MSVC) 
  target_compile_options(game PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
  target_link_options(game PRIVATE $<$<CONFIG:DEBUG>:-fsanitize=address,undefined>)
endif()

target_link_libraries(game PRIVATE engine)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")