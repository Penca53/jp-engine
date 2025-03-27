#include "physics.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "collider.h"

namespace ng {

Physics& Physics::GetInstance() {
  static Physics instance;
  return instance;
}

void Physics::AddCollider(const Collider& collider) {
  colliders_.push_back(&collider);
}

void Physics::RemoveCollider(const Collider& collider) {
  colliders_.erase(std::remove(colliders_.begin(), colliders_.end(), &collider),
                   colliders_.end());
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