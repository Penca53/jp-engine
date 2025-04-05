#include "game_manager.h"

#include <SFML/Audio/Sound.hpp>
#include <SFML/Window/Keyboard.hpp>

#include <memory>
#include <utility>

#include "default_scene.h"
#include "engine/app.h"
#include "engine/input.h"
#include "lose_canvas.h"
#include "win_canvas.h"

namespace game {

GameManager::GameManager()
    : win_sound_(
          ng::App::GetInstance().GetMutableResourceManager().LoadSoundBuffer(
              "Win_2.wav")),
      lose_sound_(
          ng::App::GetInstance().GetMutableResourceManager().LoadSoundBuffer(
              "Loose_2.wav")) {}

void GameManager::Start() {
  auto win_canvas = std::make_unique<WinCanvas>();
  win_canvas_ = win_canvas.get();
  GetParent()->AddChild(std::move(win_canvas));

  auto lose_canvas = std::make_unique<LoseCanvas>();
  lose_canvas_ = lose_canvas.get();
  GetParent()->AddChild(std::move(lose_canvas));

  state_ = State::PLAY;
}

void GameManager::Update() {
  if (state_ == State::WON || state_ == State::LOST) {
    if (ng::GetKeyDown(sf::Keyboard::Scancode::Enter)) {
      ng::App::GetInstance().LoadScene(MakeDefaultScene());
      return;
    }
  }
}

void GameManager::Win() {
  if (state_ != State::PLAY) {
    return;
  }

  state_ = State::WON;
  win_canvas_->Enable();
  win_sound_.play();
}

void GameManager::Lose() {
  if (state_ != State::PLAY) {
    return;
  }

  state_ = State::LOST;
  lose_canvas_->Enable();
  lose_sound_.play();
}

GameManager::State GameManager::GetState() const {
  return state_;
}

}  // namespace game