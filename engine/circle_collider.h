#pragma once

#include "collider.h"

namespace ng {

// Represents a circular collision shape for collision detection.
class CircleCollider : public Collider {
 public:
  // Creates a CircleCollider with the specified radius.
  explicit CircleCollider(float radius);

  // Returns the radius of the collider.
  float GetRadius() const;

  // Checks for collision with another collider. This is used to leverage
  // double dispatch to call the specialized Collides method.
  bool Collides(const Collider& other) const override;
  // Checks for collision with another circle collider
  bool Collides(const CircleCollider& other) const override;
  // Checks for collision with another rectangle collider
  bool Collides(const RectangleCollider& other) const override;

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  // Radius of the circle collider.
  float radius_ = 0;
};

}  // namespace ng