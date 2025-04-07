#include "camera_manager.h"

#include <SFML/System/Vector2.hpp>
#include <set>

#include "camera.h"

namespace ng {

void CameraManager::OnWindowResize(sf::Vector2f size) {
  for (Camera* camera : cameras_) {
    camera->SetViewSize(size);
  }
}

const std::set<Camera*, decltype(CameraManager::cmp_ascending_draw_order)>&
CameraManager::GetCameras() const {
  return cameras_;
}

void CameraManager::AddCamera(Camera* camera) {
  cameras_.insert(camera);
}

void CameraManager::RemoveCamera(Camera* camera) {
  cameras_.erase(camera);
}

}  // namespace ng
