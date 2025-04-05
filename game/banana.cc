#include "banana.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <cstdint>
#include <memory>
#include <utility>

#include "engine/app.h"
#include "engine/circle_collider.h"
#include "engine/state.h"

namespace game {

static constexpr int32_t kAnimationTPF = 4;

Banana::IdleState::IdleState(ng::State::ID id, sf::Sprite& sprite)
    : ng::State(std::move(id)),
      animation_(sprite, "Banana/Bananas.png", kAnimationTPF) {}

void Banana::IdleState::OnEnter() {
  animation_.Start();
}

void Banana::IdleState::Update() {
  animation_.Update();
}

Banana::Banana()
    : sprite_(ng::App::GetInstance().GetMutableResourceManager().LoadTexture(
          "Banana/Bananas.png")),
      animator_(std::make_unique<IdleState>("run", sprite_)) {
  SetName("Banana");
  sprite_.setScale({2, 2});
  sprite_.setOrigin({16, 16});
  sprite_.setTextureRect(sf::IntRect({0, 0}, {32, 32}));

  auto collider = std::make_unique<ng::CircleCollider>(16.F);
  AddChild(std::move(collider));
}

bool Banana::GetIsCollected() const {
  return is_collected_;
}

void Banana::Collect() {
  is_collected_ = true;
  Destroy();
}

void Banana::Update() {
  animator_.Update();
}

void Banana::Draw(sf::RenderTarget& target) {
  target.draw(sprite_, GetGlobalTransform().getTransform());
}

}  // namespace game
