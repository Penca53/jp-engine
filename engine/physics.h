#pragma once

#include <unordered_set>

#include "collider.h"

namespace ng {

// Manages the physics simulation, handling colliders and overlap checks.
class Physics {
 public:
  // Adds a collider to the physics system.
  // If the collider already exists, it is a no-op.
  void AddCollider(const Collider* collider);
  // Removes a collider to the physics system.
  // If the collider wasn't present, it is a no-op.
  void RemoveCollider(const Collider* collider);

  // Checks for overlaps between the given collider and other colliders
  // registered in the system.
  [[nodiscard]] const Collider* Overlap(const Collider& collider) const;

 private:
  // Registered colliders in the physics system.
  std::unordered_set<const Collider*> colliders_;
};

}  // namespace ng