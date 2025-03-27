#include "circle_collider.h"

#include <iostream>

#include "rectangle_collider.h"

namespace ng {

float CircleCollider::GetRadius() const {
  return radius_;
}

CircleCollider::CircleCollider(float radius) : radius_(radius) {
  SetName("CircleCollider");
}

bool CircleCollider::Collides(const Collider& other) const {
  return other.Collides(*this);
}

bool CircleCollider::Collides(const CircleCollider& other) const {
  float distanceSquared = (GetGlobalTransform().getPosition() -
                           other.GetGlobalTransform().getPosition())
                              .lengthSquared();
  return distanceSquared <=
         (radius_ + other.radius_) * (radius_ + other.radius_);
}

bool CircleCollider::Collides(const RectangleCollider& other) const {
  sf::Vector2f pos = GetGlobalTransform().getPosition();
  sf::Vector2f otherPos = other.GetGlobalTransform().getPosition();

  float left = otherPos.x -
               other.GetSize().x * other.GetGlobalTransform().getScale().x / 2;
  float right = otherPos.x +
                other.GetSize().x * other.GetGlobalTransform().getScale().x / 2;
  float top = otherPos.y -
              other.GetSize().y * other.GetGlobalTransform().getScale().y / 2;
  float bottom = otherPos.y + other.GetSize().y *
                                  other.GetGlobalTransform().getScale().y / 2;

  float closestX = std::max(left, std::min(pos.x, right));
  float closestY = std::max(top, std::min(pos.y, bottom));

  float dx = pos.x - closestX;
  float dy = pos.y - closestY;
  float distanceSquared = dx * dx + dy * dy;

  float radius = radius_ * std::max(GetGlobalTransform().getScale().x,
                                    GetGlobalTransform().getScale().y);
  return distanceSquared <= radius * radius;
}

void CircleCollider::Draw(sf::RenderTarget& target) {
  return;
  sf::CircleShape shape(radius_ * std::max(GetGlobalTransform().getScale().x,
                                           GetGlobalTransform().getScale().y));
  shape.setOutlineColor(sf::Color(0, 255, 0, 150));
  shape.setOutlineThickness(2);
  shape.setFillColor(sf::Color::Transparent);
  shape.setOrigin(sf::Vector2f{radius_, radius_} *
                  std::max(GetGlobalTransform().getScale().x,
                           GetGlobalTransform().getScale().y));
  target.draw(shape, GetGlobalTransform().getTransform());
}

}  // namespace ng