#pragma once

#include <unordered_set>

#include "collider.h"

namespace ng {

/// @brief Manages the physics simulation within a scene, primarily handling collision detection.
class Physics {
 public:
  /// @brief Adds a collider to the physics world for collision detection.
  /// @param collider A pointer to the Collider to add. This pointer must not be null and the Collider's lifetime should be managed externally to Physics.
  void AddCollider(const Collider* collider);

  /// @brief Removes a collider from the physics world.
  /// @param collider A pointer to the Collider to remove. This pointer must not be null and the Collider's lifetime should be managed externally to Physics.
  void RemoveCollider(const Collider* collider);

  /// @brief Checks if a given collider overlaps with any other collider currently in the physics world.
  /// @param collider The Collider to check for overlaps.
  /// @return A pointer to the first Collider that overlaps with the given collider, or nullptr if no overlap is found.
  [[nodiscard]] const Collider* Overlap(const Collider& collider) const;

 private:
  // A set containing pointers to all colliders in the physics world. The Physics class does not own these pointers.
  std::unordered_set<const Collider*> colliders_;
};

}  // namespace ng