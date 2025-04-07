#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include "collider.h"

namespace ng {

// Represents a rectangular collision shape for collision detection.
class RectangleCollider : public Collider {
 public:
  // Creates a RectangleCollider with the specified size.
  RectangleCollider(App& app, sf::Vector2f size);

  // Returns the size of the collider.
  [[nodiscard]] const sf::Vector2f& GetSize() const;

  // Checks for collision with another collider. This is used to leverage
  // double dispatch to call the specialized Collides method.
  [[nodiscard]] bool Collides(const Collider& other) const override;
  // Checks for collision with another CircleCollider.
  [[nodiscard]] bool Collides(const CircleCollider& other) const override;
  // Checks for collision with another RectangleCollider.
  [[nodiscard]] bool Collides(const RectangleCollider& other) const override;

 protected:
#ifdef DEBUG
  void Draw(sf::RenderTarget& target) override;
#endif

 private:
  // Size of the whole rectangle.
  sf::Vector2f size_;
};

}  // namespace ng