#include "score_manager.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <cstdint>
#include <string>
#include "engine/app.h"
#include "engine/layer.h"
#include "engine/resource_manager.h"

namespace game {

ScoreManager::ScoreManager()
    : score_text_(
          ng::ResourceManager::GetInstance().LoadFont("Roboto-Regular.ttf")) {
  SetLayer(ng::Layer::kUI);

  UpdateUI();
}

void ScoreManager::AddScore(int32_t score) {
  score_ += score;
  UpdateUI();
}

void ScoreManager::Update() {
  float width =
      static_cast<float>(ng::App::GetInstance().GetWindow().getSize().y);
  SetLocalPosition({0, -((width / 2.F) - 8)});
}

void ScoreManager::Draw(sf::RenderTarget& target) {
  target.draw(score_text_, GetGlobalTransform().getTransform());
}

void ScoreManager::UpdateUI() {
  score_text_.setString(std::to_string(score_));
  score_text_.setOrigin(score_text_.getGlobalBounds().size / 2.F);
}

}  // namespace game