#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/tilemap.h"

namespace game {

class Goomba : public ng::Node {
 public:
  explicit Goomba(const ng::Tilemap& tilemap);
  bool GetIsDead() const;
  void TakeDamage();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  class RunState : public ng::State {
   public:
    RunState(ng::State::ID id, sf::Sprite& sprite);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class HitState : public ng::State {
   public:
    HitState(ng::State::ID id, sf::Sprite& sprite, ng::Node& node);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    void Die();

    ng::SpriteSheetAnimation animation_;
    ng::Node* node_ = nullptr;
    sf::Sound sound_;
  };

  sf::RectangleShape shape_;
  sf::Vector2f direction_{-1, 0};
  sf::Vector2f velocity_;
  const ng::Tilemap* tilemap_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  bool is_on_ground_ = false;
  sf::Sprite sprite_;
  bool is_dead_ = false;
  ng::FSM animator_;
};

}  // namespace game