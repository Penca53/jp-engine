#pragma once

#include <unordered_set>

#include "collider.h"

namespace ng {

// Provides a singleton interface for handling physics interactions within the
// engine.
class Physics {
 public:
  // Returns the singleton instance of the Physics.
  static Physics& GetInstance();

  Physics(const Physics& other) = delete;
  Physics& operator=(const Physics& other) = delete;
  Physics(Physics&& other) = delete;
  Physics& operator=(Physics&& other) = delete;

  // Adds a collider to the physics system.
  // If the collider already exists, it is a no-op.
  void AddCollider(const Collider& collider);
  // Removes a collider to the physics system.
  // If the collider wasn't present, it is a no-op.
  void RemoveCollider(const Collider& collider);
  // Checks for overlaps between the given collider and other colliders
  // registered in the system.
  const Collider* Overlap(const Collider& collider);

 private:
  Physics() = default;
  ~Physics() = default;

  // Registered colliders in the physics system.
  std::unordered_set<const Collider*> colliders_;
};

}  // namespace ng