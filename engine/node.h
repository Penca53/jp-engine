#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "derived.h"
#include "layer.h"

namespace ng {

class App;
class Camera;

// Represents a node in a hierarchical scene graph, providing transformation
// and rendering capabilities.
// The Node class forms the base for all objects within the game's scene graph.
// It handles transformations, parenting, child management, layering, and
// rendering.
// Nodes can have children, forming a tree-like structure. Transformations are
// hierarchical, meaning that a node's global transform is affected by its
// parent's transform.
class Node {
 public:
  // App needs to be able to call InternalStart, InternalUpdate, and
  // InternalDraw.
  friend class App;

  Node();
  virtual ~Node();

  Node(const Node& other) = delete;
  Node& operator=(const Node& other) = delete;
  Node(Node&& other) = delete;
  Node& operator=(Node&& other) = delete;

  // Returns the name of the node.
  [[nodiscard]] const std::string& GetName() const;
  // Sets the name of the node.
  void SetName(std::string name);
  // Returns the parent node.
  // If the parent is null, then this node is a scene root.
  [[nodiscard]] Node* GetParent() const;

  // Returns the layer of the node.
  [[nodiscard]] Layer GetLayer() const;
  // Sets the layer of the node.
  void SetLayer(Layer layer);

  // Adds a child node to the node.
  void AddChild(std::unique_ptr<Node> new_child);
  // Destroys a child node.
  void DestroyChild(const Node& child_to_destroy);
  // Destroys this node.
  void Destroy();

  // Returns the local transform of the node.
  [[nodiscard]] const sf::Transformable& GetLocalTransform() const;
  // Returns the global transform of the node.
  [[nodiscard]] const sf::Transformable& GetGlobalTransform() const;

  // Sets the local position of the node.
  void SetLocalPosition(sf::Vector2f position);
  // Sets the local rotation of the node.
  void SetLocalRotation(sf::Angle rotation);
  // Sets the local scale of the node.
  void SetLocalScale(sf::Vector2f scale);
  // Translates the node by the given delta.
  void Translate(sf::Vector2f delta);

  // Returns a child node of the specified type, if any.
  template <Derived<Node> T>
  [[nodiscard]] T* GetChild() {
    for (const auto& child : children_) {
      T* c = dynamic_cast<T*>(child.get());
      if (c != nullptr) {
        return c;
      }
    }

    return nullptr;
  }

 protected:
  // Called when the node is actually added to the scene graph.
  // Start is not called right after a Node is constructed, but only after the
  // Node is actually added to the children container of the parent Node.
  // This behaviour is useful to ensure that once Start is called, the scene
  // graph is completely built and the Node is fully initialized.
  virtual void Start();
  // Called during each update cycle of the node.
  virtual void Update();
  // Called to render the node.
  virtual void Draw(sf::RenderTarget& target);

 private:
  void EraseDestroyedChildren();
  void AddQueuedChildren();

  void InternalStart();
  void InternalUpdate();
  void InternalDraw(const Camera& camera, sf::RenderTarget& target);

  void DirtyGlobalTransform();

  // The name of the node.
  std::string name_;
  // The local transform of the node.
  sf::Transformable local_transform_;
  // The global transform of the node.
  mutable sf::Transformable global_transform_;
  // Flag indicating if the global transform needs to be recomputed.
  mutable bool is_global_transform_dirty_ = false;
  // Parent node.
  Node* parent_ = nullptr;
  // Child nodes.
  // The container is a vector because the order of children determines the
  // update and draw order (lower index = earlier update / draw).
  std::vector<std::unique_ptr<Node>> children_;
  // Indices of child nodes pending to be erased.
  // Nodes are erased at the beginning of the following Update call.
  // The game logic runs in the Update phase, so deleting a node straight away
  // would mean deleting an element from the children collection that is being
  // iterated on, resulting in disaster, at best.
  std::vector<size_t> children_to_erase_;
  // Child nodes pending to be added.
  // Nodes are erased at the beginning of the following Update call.
  // The game logic runs in the Update phase, so adding a node straight away
  // would mean adding an element to the children collection that is being
  // iterated on, resulting in disaster, at best.
  std::vector<std::unique_ptr<Node>> children_to_add_;
  // The layer of the node (i.e. used for conditional rendering).
  Layer layer_ = Layer::kDefault;
};

}  // namespace ng