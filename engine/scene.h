#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <string>
#include <unordered_set>

#include "camera.h"
#include "camera_manager.h"
#include "node.h"
#include "physics.h"

namespace ng {

class App;

class Scene {
 public:
  // App needs to be able to call InternalOnAdd, InternalUpdate,
  // InternalDraw, and InternalOnDestroy.
  friend class App;
  // Node needs to be able to call RegisterNode, and UnregisterNode.
  friend class Node;

  explicit Scene(App* app);

  // Returns the name of the scene.
  [[nodiscard]] const std::string& GetName() const;
  // Sets the name of the scene.
  void SetName(std::string name);

  [[nodiscard]] CameraManager& GetCameraManager();
  [[nodiscard]] const Physics& GetPhysics();
  [[nodiscard]] Physics& GetMutablePhysics();

  // Adds a child node to the scene.
  void AddChild(std::unique_ptr<Node> new_child);

  template <Derived<Node> T, typename... Args>
  T& MakeChild(Args&&... args) {
    return root_->MakeChild<T>(std::forward<Args>(args)...);
  }

  bool IsValid(const Node* node) const;

  void OnWindowResize(sf::Vector2u new_size);

 private:
  void InternalOnAdd();
  void InternalUpdate();
  void InternalDraw(sf::RenderTarget& target);
  void InternalOnDestroy();

  void RegisterNode(const Node* node);
  void UnregisterNode(const Node* node);

  // The name of the scene.
  std::string name_;

  // Note: Cameras must be declared before scene because of destruction order.
  // The cameras in the scene will unregister in their destructor,
  // so their destructor must be called before the destructor of the cameras field).
  // Contains all the active cameras in the loaded scene.
  CameraManager camera_manager_;

  // Note: Physics must be declared before scene because of destruction order.
  // The colliders in the scene will unregister in their destructor,
  // so their destructor must be called before the destructor of the physics field).
  // Manages the physics simulation in the application.
  Physics physics_;

  std::unordered_set<const Node*> scene_nodes_;

  // Proxied root node.
  // Having an "hidden" root allows to delete any object in the scene, since
  // the root is the top-most node that contains all the actual scene nodes.
  std::unique_ptr<Node> root_;
};

}  // namespace ng