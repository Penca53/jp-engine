#include "win_canvas.h"

#include "engine/app.h"
#include "engine/resource_manager.h"

namespace game {

WinCanvas::WinCanvas()
    : title_text_(
          ng::ResourceManager::GetInstance().LoadFont("Roboto-Regular.ttf")),
      restart_text_(
          ng::ResourceManager::GetInstance().LoadFont("Roboto-Regular.ttf")) {
  SetName("WinCanvas");
  SetLayer(ng::Layer::kUI);

  background_.setSize(
      static_cast<sf::Vector2f>(ng::App::GetInstance().GetWindow().getSize()));
  background_.setOrigin(static_cast<sf::Vector2f>(
      ng::App::GetInstance().GetWindow().getSize() / 2u));
  background_.setFillColor(sf::Color(50, 50, 50, 200));

  title_text_.setString("YOU WON");
  title_text_.setCharacterSize(42);
  title_text_.setStyle(sf::Text::Bold);
  title_text_.setOrigin(title_text_.getGlobalBounds().size / 2.f);

  restart_text_.setString("(press ENTER to restart)");
  restart_text_.setCharacterSize(24);
  restart_text_.setStyle(sf::Text::Italic);
  restart_text_.setOrigin(restart_text_.getGlobalBounds().size / 2.f);
  restart_text_.setPosition(
      sf::Vector2f(0, title_text_.getGlobalBounds().size.y * 2));
}

void WinCanvas::Enable() {
  is_enabled_ = true;
}

void WinCanvas::Disable() {
  is_enabled_ = false;
}

void WinCanvas::Draw(sf::RenderTarget& target) {
  if (!is_enabled_) {
    return;
  }

  target.draw(background_, GetGlobalTransform().getTransform());
  target.draw(title_text_, GetGlobalTransform().getTransform());
  target.draw(restart_text_, GetGlobalTransform().getTransform());
}

}  // namespace game