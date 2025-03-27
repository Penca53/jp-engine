#pragma once

#include <SFML/Graphics.hpp>

#include "node.h"

namespace ng {

class CircleCollider;
class RectangleCollider;

class Collider : public Node {
 public:
  virtual ~Collider();

  virtual bool Collides(const Collider& other) const = 0;
  virtual bool Collides(const CircleCollider& other) const = 0;
  virtual bool Collides(const RectangleCollider& other) const = 0;

 protected:
  void Start() override;
};

}  // namespace ng