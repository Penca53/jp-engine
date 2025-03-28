#include "goomba.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <cmath>
#include <cstdint>
#include <memory>
#include <utility>
#include "engine/collider.h"
#include "engine/node.h"
#include "engine/physics.h"
#include "engine/rectangle_collider.h"
#include "engine/resource_manager.h"
#include "engine/state.h"
#include "engine/tilemap.h"
#include "mario.h"
#include "tile_id.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

Goomba::RunState::RunState(ng::State::ID id, sf::Sprite& sprite)
    : ng::State(std::move(id)),
      animation_(sprite, "Mushroom/Run (32x32).png", kAnimationTPF) {}

void Goomba::RunState::OnEnter() {
  animation_.Start();
}

void Goomba::RunState::Update() {
  animation_.Update();
}

Goomba::HitState::HitState(ng::State::ID id, sf::Sprite& sprite, ng::Node& node)
    : ng::State(std::move(id)),
      animation_(sprite, "Mushroom/Hit.png", kAnimationTPF),
      node_(&node),
      sound_(ng::ResourceManager::GetInstance().LoadSoundBuffer(
          "Mushroom/Hit_2.wav")) {
  animation_.RegisterOnEndCallback([this]() { Die(); });
}

void Goomba::HitState::OnEnter() {
  animation_.Start();
  sound_.play();
}

void Goomba::HitState::Update() {
  animation_.Update();
}

void Goomba::HitState::Die() {
  node_->Destroy();
}

Goomba::Goomba(const ng::Tilemap& tilemap)
    : shape_({64.F, 64.F}),
      tilemap_(&tilemap),
      sprite_(ng::ResourceManager::GetInstance().LoadTexture(
          "Mushroom/Run (32x32).png")),
      animator_(std::make_unique<RunState>("run", sprite_)) {
  SetName("Goomba");
  shape_.setFillColor(sf::Color(105, 58, 34));
  shape_.setOrigin({32, 32});

  sprite_.setScale({2, 2});
  sprite_.setOrigin({16, 16});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));

  auto collider = std::make_unique<ng::RectangleCollider>(sf::Vector2f(32, 32));
  collider->SetLocalPosition({0, 16});
  collider_ = collider.get();
  AddChild(std::move(collider));

  animator_.AddState(std::make_unique<HitState>("hit", sprite_, *this));

  animator_.AddTransition(
      ng::Transition("run", "hit", [&]() -> bool { return is_dead_; }));
}

bool Goomba::GetIsDead() const {
  return is_dead_;
}

void Goomba::TakeDamage() {
  if (is_dead_) {
    return;
  }

  is_dead_ = true;
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

void Goomba::Update() {
  animator_.Update();

  if (is_dead_) {
    return;
  }

  velocity_.x = direction_.x * 2;
  velocity_.y += 0.5;

  sf::Vector2f old_pos = collider_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos = old_pos + velocity_;

  sf::Vector2f col_size = collider_->GetSize() / 2.F;
  sf::Vector2f tilemap_size = sf::Vector2f(tilemap_->GetTileSize());

  static constexpr float kEps = 0.001;
  sf::Vector2f top_left = {new_pos.x - (col_size.x - kEps),
                           old_pos.y - (col_size.y - kEps)};
  sf::Vector2f middle_left = {new_pos.x + (col_size.x - kEps),
                              old_pos.y + (0 - kEps)};
  sf::Vector2f bottom_left = {new_pos.x - (col_size.x - kEps),
                              old_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(middle_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_left)) {
    is_dead_ = true;
    return;
  }

  if (velocity_.x < 0 && (DoesCollide(top_left, *tilemap_) ||
                          DoesCollide(middle_left, *tilemap_) ||
                          DoesCollide(bottom_left, *tilemap_))) {
    new_pos.x =
        std::ceil(top_left.x / tilemap_size.x) * tilemap_size.x + col_size.x;
    velocity_.x = 0;
    direction_.x = 1;
  }

  sf::Vector2f top_right = {new_pos.x + (col_size.x - kEps),
                            old_pos.y - (col_size.y - kEps)};
  sf::Vector2f middle_right = {new_pos.x + (col_size.x - kEps),
                               old_pos.y + (0 - kEps)};
  sf::Vector2f bottom_right = {new_pos.x + (col_size.x - kEps),
                               old_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_right) ||
      !tilemap_->IsWithinWorldBounds(middle_right) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    is_dead_ = true;
    return;
  }

  if (velocity_.x > 0 && (DoesCollide(top_right, *tilemap_) ||
                          DoesCollide(middle_right, *tilemap_) ||
                          DoesCollide(bottom_right, *tilemap_))) {
    new_pos.x =
        std::floor(top_right.x / tilemap_size.x) * tilemap_size.x - col_size.x;
    velocity_.x = 0;
    direction_.x = -1;
  }

  top_left = {new_pos.x - (col_size.x - kEps), new_pos.y - (col_size.y - kEps)};
  top_right = {new_pos.x + (col_size.x - kEps),
               new_pos.y - (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(top_right)) {
    is_dead_ = true;
    return;
  }

  if (velocity_.y < 0 &&
      (DoesCollide(top_left, *tilemap_) || DoesCollide(top_right, *tilemap_))) {
    new_pos.y =
        std::ceil(top_left.y / tilemap_size.y) * tilemap_size.y + col_size.y;
    velocity_.y = 0;
  }

  bottom_left = {new_pos.x - (col_size.x - kEps),
                 new_pos.y + (col_size.y - kEps)};
  bottom_right = {new_pos.x + (col_size.x - kEps),
                  new_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(bottom_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_right)) {
    is_dead_ = true;
    return;
  }

  if (velocity_.y > 0 && (DoesCollide(bottom_left, *tilemap_) ||
                          DoesCollide(bottom_right, *tilemap_))) {
    new_pos.y = std::floor(bottom_left.y / tilemap_size.y) * tilemap_size.y -
                col_size.y;
    velocity_.y = 0;
    is_on_ground_ = true;
  } else {
    is_on_ground_ = false;
  }

  SetLocalPosition(new_pos - sf::Vector2f{0, 16});

  const ng::Collider* other = ng::Physics::GetInstance().Overlap(*collider_);
  if (other != nullptr) {
    if (other->GetParent()->GetName() == "Mario") {
      auto* mario = dynamic_cast<Mario*>(other->GetParent());
      if (mario->GetVelocity().y <= 0) {
        mario->TakeDamage();
      }
    } else if (other->GetParent()->GetName() == "Goomba") {
      auto* goomba = dynamic_cast<Goomba*>(other->GetParent());
      if (!goomba->GetIsDead()) {
        goomba->direction_.x = -goomba->direction_.x;
        direction_.x = -direction_.x;
      }
    }
  }
}

void Goomba::Draw(sf::RenderTarget& target) {
  sprite_.setScale(sf::Vector2f{-direction_.x * 2, 2.F});
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game