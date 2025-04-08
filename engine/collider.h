#pragma once

#include "node.h"

namespace ng {

class CircleCollider;
class RectangleCollider;

// Represents a base abstract class for collision shapes. It enforces the
// implementation of collision detection methods for various collider types.
class Collider : public Node {
 public:
  explicit Collider(App* app);

  // Checks for collision with another collider. This is used to leverage
  // double dispatch to call the specialized Collides method.
  [[nodiscard]] virtual bool Collides(const Collider& other) const = 0;

  // Collision detection between specific shapes.
  [[nodiscard]] virtual bool Collides(const CircleCollider& other) const = 0;
  [[nodiscard]] virtual bool Collides(const RectangleCollider& other) const = 0;

 protected:
  void OnAdd() override;
  void OnDestroy() override;
};

}  // namespace ng