#include "scene.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <string>
#include <utility>

#include "layer.h"
#include "node.h"

namespace ng {

Scene::Scene() {
  root_ = std::make_unique<Node>();
  // Render all layers.
  root_->SetLayer(static_cast<Layer>(~0ULL));
  root_->SetName("SceneRoot");
}

const std::string& Scene::GetName() const {
  return name_;
}

void Scene::SetName(std::string name) {
  name_ = std::move(name);
}

void Scene::AddChild(std::unique_ptr<Node> new_child) {
  root_->AddChild(std::move(new_child));
}

void Scene::InternalStart() {
  root_->InternalStart();
}

void Scene::InternalUpdate() {
  root_->InternalUpdate();
}

void Scene::InternalDraw(const Camera& camera, sf::RenderTarget& target) {
  root_->InternalDraw(camera, target);
}

}  // namespace ng
