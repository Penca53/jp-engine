#pragma once

#include <set>

#include "camera.h"

namespace ng {

class CameraManager {
  friend class Camera;

 public:
  class DrawOrderCompare {
   public:
    bool operator()(const Camera* a, const Camera* b) const {
      if (a->GetDrawOrder() < b->GetDrawOrder()) {
        return true;
      }
      if (a->GetDrawOrder() > b->GetDrawOrder()) {
        return false;
      }

      return a < b;
    };
  };

  void OnWindowResize(sf::Vector2f size);
  [[nodiscard]] const std::set<Camera*, DrawOrderCompare>& GetCameras() const;

 private:
  void AddCamera(Camera* camera);
  void RemoveCamera(Camera* camera);

  std::set<Camera*, DrawOrderCompare> cameras_;
};

}  // namespace ng