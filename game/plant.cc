#include "plant.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "engine/input.h"
#include "engine/physics.h"
#include "engine/rectangle_collider.h"
#include "engine/resource_manager.h"
#include "mario.h"
#include "plant_bullet.h"
#include "tile_id.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

Plant::IdleState::IdleState(ng::State::ID id, sf::Sprite& sprite)
    : ng::State(id),
      animation_(sprite, "Plant/Idle (44x42).png", kAnimationTPF, {44, 42}) {}

void Plant::IdleState::OnEnter() {
  animation_.Start();
}

void Plant::IdleState::Update() {
  animation_.Update();
}

Plant::AttackState::AttackState(ng::State::ID id, sf::Sprite& sprite,
                                const ng::Node& node,
                                const ng::Tilemap& tilemap,
                                sf::Vector2f direction, bool& attack_completed)
    : ng::State(id),
      animation_(sprite, "Plant/Attack (44x42).png", kAnimationTPF, {44, 42}),
      node_(&node),
      tilemap_(&tilemap),
      direction_(direction),
      attack_completed_(&attack_completed) {
  animation_.RegisterOnEndCallback(
      [this] -> void { *attack_completed_ = true; });
}

void Plant::AttackState::OnEnter() {
  animation_.Start();
}

void Plant::AttackState::Update() {
  animation_.Update();
  if (animation_.GetFrameIndex() == 4 && animation_.GetTicksCounter() == 0) {
    Attack();
  }
}

void Plant::AttackState::OnExit() {
  *attack_completed_ = false;
}

void Plant::AttackState::Attack() {
  auto bullet = std::make_unique<PlantBullet>(*tilemap_, direction_);
  bullet->SetLocalPosition(node_->GetLocalTransform().getPosition() +
                           sf::Vector2f{-16.f, -6.f});
  node_->GetParent()->AddChild(std::move(bullet));
}

Plant::HitState::HitState(ng::State::ID id, sf::Sprite& sprite, ng::Node& node)
    : ng::State(id),
      animation_(sprite, "Plant/Hit (44x42).png", kAnimationTPF, {44, 42}),
      node_(&node),
      sound_(ng::ResourceManager::GetInstance().LoadSoundBuffer(
          "Mushroom/Hit_2.wav")) {
  animation_.RegisterOnEndCallback([this] -> void { Die(); });
}

void Plant::HitState::OnEnter() {
  animation_.Start();
  sound_.play();
}

void Plant::HitState::Update() {
  animation_.Update();
}

void Plant::HitState::Die() {
  node_->Destroy();
}

Plant::Plant(const ng::Tilemap& tilemap)
    : sprite_(ng::ResourceManager::GetInstance().LoadTexture(
          "Plant/Idle (44x42).png")),
      animator_(std::make_unique<IdleState>("idle", sprite_)),
      tilemap_(&tilemap) {
  SetName("Plant");
  sprite_.setScale({2, 2});
  sprite_.setOrigin({22, 21});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {44, 42}));

  auto collider = std::make_unique<ng::RectangleCollider>(sf::Vector2f(40, 42));
  collider->SetLocalPosition({8, 0});
  collider_ = collider.get();
  AddChild(std::move(collider));

  animator_.AddState(std::make_unique<AttackState>(
      "attack", sprite_, *this, *tilemap_, direction_, attack_completed_));
  animator_.AddState(std::make_unique<HitState>("hit", sprite_, *this));

  animator_.AddTransition(
      ng::Transition("idle", "hit", [&]() -> bool { return is_dead_; }));
  animator_.AddTransition(ng::Transition(
      "idle", "attack", [&]() -> bool { return begin_attack_; }));
  animator_.AddTransition(ng::Transition(
      "attack", "idle", [&]() -> bool { return attack_completed_; }));
  animator_.AddTransition(
      ng::Transition("attack", "hit", [&]() -> bool { return is_dead_; }));
}

bool Plant::GetIsDead() const {
  return is_dead_;
}

void Plant::TakeDamage() {
  if (is_dead_) {
    return;
  }

  is_dead_ = true;
}

void Plant::Update() {
  animator_.Update();

  if (is_dead_) {
    return;
  }

  static constexpr int32_t kAttackCooldown = 240;
  if (attack_timer_ > 0) {
    --attack_timer_;
    begin_attack_ = false;
  } else {
    begin_attack_ = true;
    attack_timer_ = kAttackCooldown;
  }

  const ng::Collider* other = ng::Physics::GetInstance().Overlap(*collider_);
  if (other) {
    if (other->GetParent()->GetName() == "Mario") {
      Mario* mario = static_cast<Mario*>(other->GetParent());
      if (mario->GetVelocity().y <= 0) {
        mario->TakeDamage();
      }
    }
  }
}

void Plant::Draw(sf::RenderTarget& target) {
  sprite_.setScale(sf::Vector2f{-direction_.x * 2, 2.f});
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game