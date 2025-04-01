#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/tilemap.h"

namespace game {

class Plant : public ng::Node {
 public:
  explicit Plant(const ng::Tilemap& tilemap);
  bool GetIsDead() const;
  void TakeDamage();

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  class IdleState : public ng::State {
   public:
    IdleState(ng::State::ID id, sf::Sprite& sprite);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class AttackState : public ng::State {
   public:
    AttackState(ng::State::ID id, sf::Sprite& sprite, const ng::Node& node,
                const ng::Tilemap& tilemap, sf::Vector2f direction,
                bool& attack_completed);

   protected:
    void OnEnter() override;

    void Update() override;

    void OnExit() override;

   private:
    void Attack();

    ng::SpriteSheetAnimation animation_;
    const ng::Node* node_ = nullptr;
    const ng::Tilemap* tilemap_ = nullptr;
    sf::Vector2f direction_;
    bool* attack_completed_ = nullptr;
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

  sf::Vector2f direction_{-1, 0};
  const ng::Tilemap* tilemap_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  bool is_dead_ = false;
  int32_t attack_timer_ = 0;
  ng::FSM animator_;
  bool begin_attack_ = false;
  bool attack_completed_ = false;
};

}  // namespace game