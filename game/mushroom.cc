#include "mushroom.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cmath>
#include <cstdint>
#include <memory>
#include <utility>

#include "engine/app.h"
#include "engine/collider.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/state.h"
#include "engine/tilemap.h"
#include "engine/transition.h"
#include "player.h"
#include "tile_id.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

Mushroom::RunState::RunState(ng::State<Context>::ID id, sf::Sprite& sprite)
    : ng::State<Context>(std::move(id)),
      animation_(sprite, "Mushroom/Run (32x32).png", kAnimationTPF) {}

void Mushroom::RunState::OnEnter() {
  animation_.Start();
}

void Mushroom::RunState::Update() {
  animation_.Update();
}

Mushroom::HitState::HitState(ng::State<Context>::ID id, sf::Sprite& sprite,
                             ng::Node& node)
    : ng::State<Context>(std::move(id)),
      animation_(sprite, "Mushroom/Hit.png", kAnimationTPF),
      sound_(ng::App::GetInstance().GetMutableResourceManager().LoadSoundBuffer(
          "Mushroom/Hit_2.wav")),
      node_(&node) {
  animation_.RegisterOnEndCallback([this]() -> void { Die(); });
}

void Mushroom::HitState::OnEnter() {
  animation_.Start();
  sound_.play();
}

void Mushroom::HitState::Update() {
  animation_.Update();
}

void Mushroom::HitState::Die() {
  node_->Destroy();
}

Mushroom::Mushroom(const ng::Tilemap& tilemap)
    : tilemap_(&tilemap),
      sprite_(ng::App::GetInstance().GetMutableResourceManager().LoadTexture(
          "Mushroom/Run (32x32).png")),
      animator_(context_, std::make_unique<RunState>("run", sprite_)) {
  SetName("Mushroom");

  sprite_.setScale({2, 2});
  sprite_.setOrigin({16, 16});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));

  auto collider = std::make_unique<ng::RectangleCollider>(sf::Vector2f(32, 32));
  collider->SetLocalPosition({0, 16});
  collider_ = collider.get();
  AddChild(std::move(collider));

  animator_.AddState(std::make_unique<HitState>("hit", sprite_, *this));

  animator_.AddTransition(ng::Transition<Context>(
      "run", "hit", [](Context context) -> bool { return context.is_dead; }));
}

bool Mushroom::GetIsDead() const {
  return context_.is_dead;
}

void Mushroom::TakeDamage() {
  context_.is_dead = true;
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

void Mushroom::Update() {  // NOLINT
  animator_.Update();

  if (context_.is_dead) {
    return;
  }

  velocity_.x = direction_.x * 2;
  velocity_.y += 0.5;

  sf::Vector2f old_pos = collider_->GetGlobalTransform().getPosition();
  sf::Vector2f new_pos = old_pos + velocity_;

  sf::Vector2f col_size = collider_->GetSize() / 2.F;
  sf::Vector2f tilemap_size = sf::Vector2f(tilemap_->GetTileSize());

  static constexpr float kEps = 0.001F;
  sf::Vector2f top_left = {new_pos.x - (col_size.x - kEps),
                           old_pos.y - (col_size.y - kEps)};
  sf::Vector2f middle_left = {new_pos.x + (col_size.x - kEps),
                              old_pos.y + (0 - kEps)};
  sf::Vector2f bottom_left = {new_pos.x - (col_size.x - kEps),
                              old_pos.y + (col_size.y - kEps)};

  if (!tilemap_->IsWithinWorldBounds(top_left) ||
      !tilemap_->IsWithinWorldBounds(middle_left) ||
      !tilemap_->IsWithinWorldBounds(bottom_left)) {
    context_.is_dead = true;
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
    context_.is_dead = true;
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
    context_.is_dead = true;
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
    context_.is_dead = true;
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

  const ng::Collider* other =
      ng::App::GetInstance().GetPhysics().Overlap(*collider_);
  if (other != nullptr) {
    if (other->GetParent()->GetName() == "Player") {
      auto* player = dynamic_cast<Player*>(other->GetParent());
      if (player->GetVelocity().y <= 0) {
        player->TakeDamage();
      }
    } else if (other->GetParent()->GetName() == "Mushroom") {
      auto* mushroom = dynamic_cast<Mushroom*>(other->GetParent());
      if (!mushroom->GetIsDead()) {
        mushroom->direction_.x = -mushroom->direction_.x;
        direction_.x = -direction_.x;
      }
    }
  }
}

void Mushroom::Draw(sf::RenderTarget& target) {
  sprite_.setScale(sf::Vector2f{-direction_.x * 2, 2.F});
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game