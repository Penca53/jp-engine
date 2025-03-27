#include "default_scene.h"
#include "engine/app.h"

#include <SFML/System/Vector2.hpp>
#include <cstdlib>

int main() {
  static constexpr sf::Vector2u kWindowSize = {832U, 640U};
  ng::App::GetInstance()
      .SetWindowTitle("Platformer")
      .SetWindowSize(kWindowSize)
      .LoadScene(game::MakeDefaultScene())
      .Run(60, 60);
  return EXIT_SUCCESS;
}
