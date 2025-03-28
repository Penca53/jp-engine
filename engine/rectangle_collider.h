#pragma once

#include "collider.h"

namespace ng {

class RectangleCollider : public Collider {
 public:
  explicit RectangleCollider(sf::Vector2f size);

  const sf::Vector2f& GetSize() const;

  bool Collides(const Collider& other) const override;
  bool Collides(const CircleCollider& other) const override;
  bool Collides(const RectangleCollider& other) const override;

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  sf::Vector2f size_;
};

}  // namespace ng