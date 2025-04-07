#include "rectangle_collider.h"

#include "app.h"
#include "circle_collider.h"
#include "collider.h"

#ifdef DEBUG
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#endif
#include <SFML/System/Vector2.hpp>

namespace ng {

RectangleCollider::RectangleCollider(App& app, sf::Vector2f size)
    : Collider(app), size_(size) {
  SetName("RectangleCollider");
}

const sf::Vector2f& RectangleCollider::GetSize() const {
  return size_;
}

bool RectangleCollider::Collides(const Collider& other) const {
  return other.Collides(*this);
}

bool RectangleCollider::Collides(const CircleCollider& other) const {
  return other.Collides(*this);
}

bool RectangleCollider::Collides(const RectangleCollider& other) const {
  sf::Vector2f pos = GetGlobalTransform().getPosition();
  sf::Vector2f otherPos = other.GetGlobalTransform().getPosition();
  bool AisToTheRightOfB =
      pos.x - size_.x * GetGlobalTransform().getScale().x / 2 >
      otherPos.x + other.size_.x * other.GetGlobalTransform().getScale().x / 2;
  bool AisToTheLeftOfB =
      pos.x + size_.x * GetGlobalTransform().getScale().x / 2 <
      otherPos.x - other.size_.x * other.GetGlobalTransform().getScale().x / 2;
  bool AisBelowB =
      pos.y - size_.y * GetGlobalTransform().getScale().y / 2 >
      otherPos.y + other.size_.y * other.GetGlobalTransform().getScale().y / 2;
  bool AisAboveB =
      pos.y + size_.y * GetGlobalTransform().getScale().y / 2 <
      otherPos.y - other.size_.y * other.GetGlobalTransform().getScale().y / 2;
  return !(AisToTheRightOfB || AisToTheLeftOfB || AisAboveB || AisBelowB);
}

#ifdef DEBUG
void RectangleCollider::Draw(sf::RenderTarget& target) {
  sf::RectangleShape shape(
      size_.componentWiseMul(GetGlobalTransform().getScale()));
  shape.setOutlineColor(sf::Color(0, 255, 0, 150));
  shape.setOutlineThickness(2);
  shape.setFillColor(sf::Color::Transparent);
  shape.setOrigin(size_.componentWiseMul(GetGlobalTransform().getScale()) /
                  2.F);
  target.draw(shape, GetGlobalTransform().getTransform());
}
#endif

}  // namespace ng