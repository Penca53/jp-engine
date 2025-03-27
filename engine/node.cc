#include "node.h"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>

#include "app.h"

namespace ng {

Node::Node() {
  App::GetInstance().AddValidNode(*this);
}

Node::~Node() {
  App::GetInstance().RemoveValidNode(*this);
}

const std::string& Node::GetName() const {
  return name_;
}

void Node::SetName(std::string name) {
  name_ = std::move(name);
}

Node* Node::GetParent() const {
  return parent_;
}

Layer Node::GetLayer() const {
  return layer_;
}

void Node::SetLayer(Layer layer) {
  layer_ = layer;
}

void Node::AddChild(std::unique_ptr<Node> new_child) {
  new_child->parent_ = this;
  new_child->DirtyGlobalTransform();
  children_to_add_.push_back(std::move(new_child));
}

void Node::DestroyChild(const Node& child_to_destroy) {
  for (size_t i = 0; i < children_.size(); ++i) {
    if (children_[i].get() == &child_to_destroy) {
      children_to_erase_.push_back(i);
      break;
    }
  }
}

void Node::Destroy() {
  if (parent_) {
    parent_->DestroyChild(*this);
  } else {
    App::GetInstance().UnloadScene();
  }
}

const sf::Transformable& Node::GetLocalTransform() const {
  return local_transform_;
}

const sf::Transformable& Node::GetGlobalTransform() const {
  if (is_global_transform_dirty_) {
    if (parent_) {
      sf::Transform new_transform =
          parent_->GetGlobalTransform().getTransform() *
          GetLocalTransform().getTransform();

      float a00 = new_transform.getMatrix()[0];
      float a01 = new_transform.getMatrix()[4];
      float a02 = new_transform.getMatrix()[12];
      float a10 = new_transform.getMatrix()[1];
      float a11 = new_transform.getMatrix()[5];
      float a12 = new_transform.getMatrix()[13];

      sf::Angle rotation(sf::radians(std::atan2(-a01, a00)));
      sf::Vector2 scale(std::sqrt(a00 * a00 + a10 * a10),
                        std::sqrt(a01 * a01 + a11 * a11));
      sf::Vector2f position(a02, a12);

      global_transform_.setRotation(rotation);
      global_transform_.setScale(scale);
      global_transform_.setPosition(position);
    } else {
      global_transform_ = GetLocalTransform();
    }

    is_global_transform_dirty_ = false;
  }

  return global_transform_;
}

void Node::SetLocalPosition(sf::Vector2f position) {
  local_transform_.setPosition(position);
  DirtyGlobalTransform();
}

void Node::SetLocalRotation(sf::Angle rotation) {
  local_transform_.setRotation(rotation);
  DirtyGlobalTransform();
}

void Node::SetLocalScale(sf::Vector2f scale) {
  local_transform_.setScale(scale);
  DirtyGlobalTransform();
}

void Node::Translate(sf::Vector2f delta) {
  local_transform_.move(delta);
  DirtyGlobalTransform();
}

void Node::Start() {}

void Node::Update() {}

void Node::Draw(sf::RenderTarget& target) {}

void Node::EraseDestroyedChildren() {
  if (children_to_erase_.size() == 0) {
    return;
  }

  auto prev_frame_children_to_erase = std::move(children_to_erase_);
  std::sort(prev_frame_children_to_erase.begin(),
            prev_frame_children_to_erase.end(), std::greater<>());
  for (size_t to_erase : prev_frame_children_to_erase) {
    children_.erase(children_.begin() + to_erase);
  }
}

void Node::AddQueuedChildren() {
  auto prev_frame_children_to_add = std::move(children_to_add_);
  for (auto& to_add : prev_frame_children_to_add) {
    Node* tmp = to_add.get();
    children_.push_back(std::move(to_add));
    tmp->InternalStart();
  }
}

void Node::InternalStart() {
  Start();
}

void Node::InternalUpdate() {
  EraseDestroyedChildren();
  AddQueuedChildren();

  Update();
  for (size_t i = 0; i < children_.size(); ++i) {
    children_[i]->InternalUpdate();
  }
}

void Node::InternalDraw(const Camera& camera, sf::RenderTarget& target) {
  if (!(std::to_underlying(layer_) & std::to_underlying(camera.GetLayer()))) {
    return;
  }

  Draw(target);
  for (size_t i = 0; i < children_.size(); ++i) {
    children_[i]->InternalDraw(camera, target);
  }
}

void Node::DirtyGlobalTransform() {
  if (is_global_transform_dirty_) {
    return;
  }

  is_global_transform_dirty_ = true;
  for (auto& child : children_) {
    child->DirtyGlobalTransform();
  }
}

}  // namespace ng