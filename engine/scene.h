#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <memory>
#include <string>

#include "camera.h"
#include "node.h"

namespace ng {

class Scene {
 public:
  // App needs to be able to call InternalStart, InternalUpdate, and
  // InternalDraw.
  friend class App;

  Scene();

  // Returns the name of the scene.
  [[nodiscard]] const std::string& GetName() const;
  // Sets the name of the scene.
  void SetName(std::string name);

  // Adds a child node to the scene.
  void AddChild(std::unique_ptr<Node> new_child);

 private:
  void InternalStart();
  void InternalUpdate();
  void InternalDraw(const Camera& camera, sf::RenderTarget& target);

  // The name of the scene.
  std::string name_;
  // Proxied root node.
  // Having an "hidden" root allows to delete any object in the scene, since
  // the root is the top-most node that contains all the actual scene nodes.
  std::unique_ptr<Node> root_;
};

}  // namespace ng