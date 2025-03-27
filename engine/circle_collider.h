#pragma once

#include "collider.h"

namespace ng {

class CircleCollider : public Collider {
 public:
  CircleCollider(float radius);

 public:
  float GetRadius() const;

  bool Collides(const Collider& other) const override;
  bool Collides(const CircleCollider& other) const override;
  bool Collides(const RectangleCollider& other) const override;

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  float radius_ = 0;
};

}  // namespace ng