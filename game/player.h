#pragma once

#include <SFML/Audio/Sound.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/rectangle_collider.h"
#include "engine/sprite_sheet_animation.h"
#include "engine/tilemap.h"
#include "game_manager.h"
#include "score_manager.h"

namespace game {

class Player : public ng::Node {
 public:
  Player(ng::Tilemap& tilemap, ScoreManager& score_manager,
         GameManager& game_manager);
  sf::Vector2f GetVelocity() const;
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

  class RunState : public ng::State {
   public:
    RunState(ng::State::ID id, sf::Sprite& sprite);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class JumpState : public ng::State {
   public:
    JumpState(ng::State::ID id, sf::Sprite& sprite);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
    sf::Sound sound_;
  };

  class FallState : public ng::State {
   public:
    FallState(ng::State::ID id, sf::Sprite& sprite);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
  };

  class HitState : public ng::State {
   public:
    HitState(ng::State::ID id, sf::Sprite& sprite, ng::Node& node,
             GameManager& game_manager);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    void Die();

    ng::SpriteSheetAnimation animation_;
    ng::Node* node_ = nullptr;
    GameManager* game_manager_ = nullptr;
  };

  sf::Vector2f velocity_;
  ng::Tilemap* tilemap_ = nullptr;
  ScoreManager* score_manager_ = nullptr;
  GameManager* game_manager_ = nullptr;
  const ng::RectangleCollider* collider_ = nullptr;
  sf::Sprite sprite_;
  bool is_on_ground_ = false;
  bool is_dead_ = false;
  bool has_won_ = false;
  ng::FSM animator_;
  sf::Sound plastic_block_sound_;
  sf::Sound banana_sound_;
};

}  // namespace game