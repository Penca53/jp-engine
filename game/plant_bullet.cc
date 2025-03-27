#include "plant_bullet.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "engine/circle_collider.h"
#include "engine/input.h"
#include "engine/physics.h"
#include "engine/resource_manager.h"
#include "engine/tilemap.h"
#include "mario.h"
#include "tile_id.h"

namespace game {

PlantBullet::PlantBullet(const ng::Tilemap& tilemap, sf::Vector2f direction)
    : sprite_(
          ng::ResourceManager::GetInstance().LoadTexture("Plant/Bullet.png")),
      tilemap_(&tilemap),
      direction_(direction) {
  SetName("PlantBullet");
  sprite_.setScale({2, 2});
  sprite_.setOrigin({8, 8});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {16, 16}));

  auto collider = std::make_unique<ng::CircleCollider>(4);
  collider_ = collider.get();
  AddChild(std::move(collider));
}

bool PlantBullet::GetIsDead() const {
  return is_dead_;
}

namespace {

bool DoesCollide(sf::Vector2f position, const ng::Tilemap& tilemap) {
  TileID id = tilemap.GetWorldTile(position).GetID();
  return id == TileID::kInvisibleBarrier ||
         (id >= TileID::kDirtTopLeft && id <= TileID::kDirtBottomRight) ||
         (id >= TileID::kStoneHorizontalLeft &&
          id <= TileID::kStoneVerticalBottom) ||
         id == TileID::kPlasticBlock;
}

}  // namespace

void PlantBullet::Update() {
  if (is_dead_) {
    return;
  }

  sf::Vector2f pos = GetGlobalTransform().getPosition();
  if (!tilemap_->IsWithinWorldBounds(pos)) {
    is_dead_ = true;
    Destroy();
    return;
  }

  if (DoesCollide(pos, *tilemap_)) {
    is_dead_ = true;
    Destroy();
    return;
  }

  static constexpr float kMovementSpeed = 6;
  Translate(direction_ * kMovementSpeed);

  const ng::Collider* other = ng::Physics::GetInstance().Overlap(*collider_);
  if (other) {
    if (other->GetParent()->GetName() == "Mario") {
      Mario* mario = static_cast<Mario*>(other->GetParent());
      mario->TakeDamage();
    }
  }
}

void PlantBullet::Draw(sf::RenderTarget& target) {
  sprite_.setScale(sf::Vector2f{-direction_.x * 2, 2.f});
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game