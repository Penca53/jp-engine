#include "default_scene.h"
#include "engine/app.h"

int main() {
  static constexpr sf::Vector2u kWindowSize = {832u, 640u};
  ng::App::GetInstance()
      .SetWindowTitle("Platformer")
      .SetWindowSize(kWindowSize)
      .LoadScene(game::MakeDefaultScene())
      .Run(60, 60);
  return EXIT_SUCCESS;
}
