#pragma once

#include <SFML/Graphics.hpp>

#include "engine/node.h"

namespace game {

class Background : public ng::Node {
 public:
  explicit Background(sf::Vector2u size);

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Vector2u size_;
  sf::Texture* texture_ = nullptr;
  sf::VertexArray image_vertices_;
  int32_t t_ = 0;
};

}  // namespace game