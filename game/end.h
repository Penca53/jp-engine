#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

#include "engine/fsm.h"
#include "engine/node.h"
#include "engine/sprite_sheet_animation.h"
#include "game_manager.h"

namespace game {

class End : public ng::Node {
 public:
  explicit End(GameManager& game_manager);

  void EndGame();

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

  class PressedState : public ng::State {
   public:
    PressedState(ng::State::ID id, sf::Sprite& sprite,
                 GameManager& game_manager);

   protected:
    void OnEnter() override;

    void Update() override;

   private:
    ng::SpriteSheetAnimation animation_;
    GameManager* game_manager_ = nullptr;
  };

  sf::Sprite sprite_;
  ng::FSM animator_;
  GameManager* game_manager_ = nullptr;
  bool is_pressed_ = false;
  bool is_end_ = false;
};

}  // namespace game