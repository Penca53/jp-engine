#include "physics.h"

#include <cassert>
#include <unordered_set>

#include "collider.h"

namespace ng {

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

void Physics::AddCollider(const Collider* collider) {
  assert(collider);
  colliders_.insert(collider);
}

void Physics::RemoveCollider(const Collider* collider) {
  assert(collider);
  colliders_.erase(collider);
}

}  // namespace ng