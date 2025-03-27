#pragma once

#include <SFML/Graphics.hpp>
#include <cstdint>

#include "engine/node.h"

namespace game {

class ScoreManager : public ng::Node {
 public:
  ScoreManager();

  void AddScore(int32_t score);

 protected:
  void Update() override;
  void Draw(sf::RenderTarget& target) override;

 private:
  void UpdateUI();

 private:
  sf::Text score_text_;
  int32_t score_ = 0;
};

}  // namespace game