#include "circle_collider.h"

#include "collider.h"
#include "rectangle_collider.h"

#ifdef DEBUG
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#endif
#include <SFML/System/Vector2.hpp>
#include <algorithm>

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
  sf::Vector2f other_pos = other.GetGlobalTransform().getPosition();

  float left = other_pos.x - (other.GetSize().x *
                              other.GetGlobalTransform().getScale().x / 2);
  float right = other_pos.x + (other.GetSize().x *
                               other.GetGlobalTransform().getScale().x / 2);
  float top = other_pos.y -
              (other.GetSize().y * other.GetGlobalTransform().getScale().y / 2);
  float bottom = other_pos.y + (other.GetSize().y *
                                other.GetGlobalTransform().getScale().y / 2);

  float closest_x = std::max(left, std::min(pos.x, right));
  float closest_y = std::max(top, std::min(pos.y, bottom));

  float diff_x = pos.x - closest_x;
  float diff_y = pos.y - closest_y;
  float distance_squared = (diff_x * diff_x) + (diff_y * diff_y);

  float radius = radius_ * std::max(GetGlobalTransform().getScale().x,
                                    GetGlobalTransform().getScale().y);
  return distance_squared <= radius * radius;
}

#ifdef DEBUG
void CircleCollider::Draw(sf::RenderTarget& target) {
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
#endif

}  // namespace ng