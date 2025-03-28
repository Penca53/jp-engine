#include "physics.h"

#include <unordered_set>

#include "collider.h"

namespace ng {

Physics& Physics::GetInstance() {
  static Physics instance;
  return instance;
}

void Physics::AddCollider(const Collider& collider) {
  colliders_.insert(&collider);
}

void Physics::RemoveCollider(const Collider& collider) {
  colliders_.erase(&collider);
}

const Collider* Physics::Overlap(const Collider& collider) {
  for (const auto* other : colliders_) {
    if (other == &collider) {
      continue;
    }

    if (collider.Collides(*other)) {
      return other;
    }
  }

  return nullptr;
}

}  // namespace ng