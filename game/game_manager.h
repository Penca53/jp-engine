#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "engine/node.h"
#include "lose_canvas.h"
#include "win_canvas.h"

namespace game {

class GameManager : public ng::Node {
 public:
  enum class State : uint8_t {
    BEGIN = 0,
    PLAY,
    WON,
    LOST,
  };

  GameManager();

  void Win();
  void Lose();

  State GetState() const;

 protected:
  void Start() override;
  void Update() override;

 private:
  State state_{};
  sf::Sound win_sound_;
  sf::Sound lose_sound_;

  WinCanvas* win_canvas_ = nullptr;
  LoseCanvas* lose_canvas_ = nullptr;
};

}  // namespace game