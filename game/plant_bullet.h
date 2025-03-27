#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "engine/circle_collider.h"
#include "engine/node.h"
#include "engine/tilemap.h"

namespace game {

class PlantBullet : public ng::Node {
 public:
  PlantBullet(const ng::Tilemap& tilemap, sf::Vector2f direction);

  bool GetIsDead() const;

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Vector2f direction_{-1, 0};
  const ng::Tilemap* tilemap_ = nullptr;
  const ng::CircleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  bool is_dead_ = false;
};

}  // namespace game