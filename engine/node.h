#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "derived.h"
#include "layer.h"

namespace ng {

class App;
class Camera;

class Node {
 public:
  friend class App;

  Node();
  virtual ~Node();

  const std::string& GetName() const;
  void SetName(std::string name);
  Node* GetParent() const;

  Layer GetLayer() const;
  void SetLayer(Layer layer);

  void AddChild(std::unique_ptr<Node> new_child);
  void DestroyChild(const Node& child_to_destroy);
  void Destroy();

  const sf::Transformable& GetLocalTransform() const;
  const sf::Transformable& GetGlobalTransform() const;

  void SetLocalPosition(sf::Vector2f position);
  void SetLocalRotation(sf::Angle rotation);
  void SetLocalScale(sf::Vector2f scale);
  void Translate(sf::Vector2f delta);

  template <Derived<Node> T>
  T* GetChild() {
    for (size_t i = 0; i < children_.size(); ++i) {
      T* child = dynamic_cast<T*>(children_[i].get());
      if (child != nullptr) {
        return child;
      }
    }

    return nullptr;
  }

 protected:
  virtual void Start();
  virtual void Update();
  virtual void Draw(sf::RenderTarget& target);

 private:
  void EraseDestroyedChildren();
  void AddQueuedChildren();

  void InternalStart();
  void InternalUpdate();
  void InternalDraw(const Camera& camera, sf::RenderTarget& target);

  void DirtyGlobalTransform();

 private:
  std::string name_;
  sf::Transformable local_transform_;
  mutable sf::Transformable global_transform_;
  mutable bool is_global_transform_dirty_ = false;
  Node* parent_ = nullptr;
  std::vector<std::unique_ptr<Node>> children_;
  std::vector<size_t> children_to_erase_;
  std::vector<std::unique_ptr<Node>> children_to_add_;
  Layer layer_ = Layer::kDefault;
};

}  // namespace ng