#include "plant.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include <cstdint>
#include <memory>
#include <utility>
#include "engine/app.h"
#include "engine/collider.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/state.h"
#include "engine/tilemap.h"
#include "plant_bullet.h"
#include "player.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

Plant::IdleState::IdleState(ng::State::ID id, sf::Sprite& sprite)
    : ng::State(std::move(id)),
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
    : ng::State(std::move(id)),
      animation_(sprite, "Plant/Attack (44x42).png", kAnimationTPF, {44, 42}),
      node_(&node),
      tilemap_(&tilemap),
      direction_(direction),
      attack_completed_(&attack_completed) {
  animation_.RegisterOnEndCallback([this]() { *attack_completed_ = true; });
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
                           sf::Vector2f{-16.F, -6.F});
  node_->GetParent()->AddChild(std::move(bullet));
}

Plant::HitState::HitState(ng::State::ID id, sf::Sprite& sprite, ng::Node& node)
    : ng::State(std::move(id)),
      animation_(sprite, "Plant/Hit (44x42).png", kAnimationTPF, {44, 42}),
      node_(&node),
      sound_(ng::App::GetInstance().GetMutableResourceManager().LoadSoundBuffer(
          "Mushroom/Hit_2.wav")) {
  animation_.RegisterOnEndCallback([this]() { Die(); });
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
    : tilemap_(&tilemap),
      sprite_(ng::App::GetInstance().GetMutableResourceManager().LoadTexture(
          "Plant/Idle (44x42).png")),
      animator_(std::make_unique<IdleState>("idle", sprite_)) {
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

  const ng::Collider* other =
      ng::App::GetInstance().GetPhysics().Overlap(*collider_);
  if (other != nullptr) {
    if (other->GetParent()->GetName() == "Player") {
      auto* player = dynamic_cast<Player*>(other->GetParent());
      if (player->GetVelocity().y <= 0) {
        player->TakeDamage();
      }
    }
  }
}

void Plant::Draw(sf::RenderTarget& target) {
  sprite_.setScale(sf::Vector2f{-direction_.x * 2, 2.F});
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game