#pragma once

#include <set>

#include "camera.h"

namespace ng {

/// @brief Manages a collection of cameras within a scene, handling resizing and providing access to them in draw order.
class CameraManager {
  /// @brief Grants Camera class access to internal methods for camera registration and unregistration.
  friend class Camera;

 public:
  /// @brief Comparator class used to sort cameras based on their draw order. If draw orders are equal, it uses pointer comparison for determinism.
  class DrawOrderCompare {
   public:
    /// @brief Compares two Camera pointers based on their draw order.
    /// @param a A pointer to the first Camera.
    /// @param b A pointer to the second Camera.
    /// @return True if camera 'a' should be drawn before camera 'b', false otherwise.
    bool operator()(const Camera* a, const Camera* b) const {
      return a->GetDrawOrder() < b->GetDrawOrder();
    };
  };

  /// @brief Called when the game window is resized. Notifies all managed cameras to update their view size.
  /// @param size The new size of the window.
  void OnWindowResize(sf::Vector2f size);

  /// @brief Returns a constant reference to the set of managed cameras, sorted by their draw order.
  /// @return A constant reference to the set of Camera pointers.
  [[nodiscard]] const std::set<Camera*, DrawOrderCompare>& GetCameras() const;

 private:
  /// @brief Adds a camera to the managed set. Called by Camera during its addition to a scene.
  /// @param camera A pointer to the Camera to add. This pointer must not be null.
  void AddCamera(Camera* camera);

  /// @brief Removes a camera from the managed set. Called by Camera during its removal from a scene.
  /// @param camera A pointer to the Camera to remove. This pointer must not be null. It it points to a Camera not currently managed, the call has no effect.
  void RemoveCamera(Camera* camera);

  // A set containing pointers to all cameras managed by this manager, sorted using DrawOrderCompare.
  std::set<Camera*, DrawOrderCompare> cameras_;
};

}  // namespace ng