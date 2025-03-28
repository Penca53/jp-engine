#include "end.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>
#include <memory>
#include <utility>
#include "engine/rectangle_collider.h"
#include "engine/resource_manager.h"
#include "engine/state.h"
#include "game_manager.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

End::IdleState::IdleState(ng::State::ID id, sf::Sprite& sprite)
    : ng::State(std::move(id)),
      animation_(sprite, "End/End (Idle).png", kAnimationTPF) {}

void End::IdleState::OnEnter() {
  animation_.Start();
}

void End::IdleState::Update() {
  animation_.Update();
}

End::PressedState::PressedState(ng::State::ID id, sf::Sprite& sprite,
                                GameManager& game_manager)
    : ng::State(std::move(id)),
      animation_(sprite, "End/End (Pressed) (64x64).png", kAnimationTPF),
      game_manager_(&game_manager) {}

void End::PressedState::OnEnter() {
  animation_.Start();
  animation_.RegisterOnEndCallback([this]() { game_manager_->Win(); });
}

void End::PressedState::Update() {
  animation_.Update();
}

End::End(GameManager& game_manager)
    : sprite_(
          ng::ResourceManager::GetInstance().LoadTexture("End/End (Idle).png")),
      animator_(std::make_unique<IdleState>("idle", sprite_)),
      game_manager_(&game_manager) {
  SetName("End");
  sprite_.setScale({2, 2});
  sprite_.setOrigin({32, 32});

  auto collider = std::make_unique<ng::RectangleCollider>(sf::Vector2f(60, 32));
  collider->SetLocalPosition({0, -20});
  AddChild(std::move(collider));

  animator_.AddState(
      std::make_unique<PressedState>("pressed", sprite_, *game_manager_));

  animator_.AddTransition(
      ng::Transition("idle", "pressed", [&]() -> bool { return is_pressed_; }));
  animator_.AddTransition(ng::Transition("pressed", "idle", [&]() -> bool {
    return game_manager_->GetState() == GameManager::State::WON;
  }));
}

void End::EndGame() {
  is_pressed_ = true;
}

void End::Update() {
  animator_.Update();
  is_pressed_ = false;
}

void End::Draw(sf::RenderTarget& target) {
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game