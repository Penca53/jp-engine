#pragma once

#include <set>

#include "camera.h"

namespace ng {

class CameraManager {
  friend class Camera;

 public:
  static constexpr auto cmp_ascending_draw_order = [](const Camera* a,
                                                      const Camera* b) {
    if (a->GetDrawOrder() < b->GetDrawOrder()) {
      return true;
    }
    if (a->GetDrawOrder() > b->GetDrawOrder()) {
      return false;
    }

    return a < b;
  };

  void OnWindowResize(sf::Vector2f size);
  [[nodiscard]] const std::set<Camera*, decltype(cmp_ascending_draw_order)>&
  GetCameras() const;

 private:
  void AddCamera(Camera* camera);
  void RemoveCamera(Camera* camera);

  std::set<Camera*, decltype(cmp_ascending_draw_order)> cameras_;
};

}  // namespace ng