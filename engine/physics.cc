#include "physics.h"

#include <unordered_set>

#include "collider.h"

namespace ng {

void Physics::AddCollider(const Collider* collider) {
  colliders_.insert(collider);
}

void Physics::RemoveCollider(const Collider* collider) {
  colliders_.erase(collider);
}

const Collider* Physics::Overlap(const Collider& collider) const {
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