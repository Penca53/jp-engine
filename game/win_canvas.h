#pragma once

#include "engine/node.h"

namespace game {

class WinCanvas : public ng::Node {
 public:
  WinCanvas();

  void Enable();
  void Disable();

 protected:
  void Draw(sf::RenderTarget& target) override;

 private:
  bool is_enabled_ = false;
  sf::RectangleShape background_;
  sf::Text title_text_;
  sf::Text restart_text_;
};

}  // namespace game