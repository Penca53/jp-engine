#pragma once

#include <vector>

#include "collider.h"

namespace ng {

class Physics {
 public:
  static Physics& GetInstance();

  void AddCollider(const Collider& collider);
  void RemoveCollider(const Collider& collider);
  const Collider* Overlap(const Collider& collider);

 private:
  Physics() = default;

  std::vector<const Collider*> colliders_;
};

}  // namespace ng