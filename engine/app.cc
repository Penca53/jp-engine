#include "app.h"

#include <SFML/System/Vector2.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <chrono>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>

#include "input.h"
#include "resource_manager.h"
#include "scene.h"

namespace ng {

App::App(sf::Vector2u window_size, const std::string& window_title) {
  window_ = sf::RenderWindow(sf::VideoMode(window_size), window_title);
}

App& App::SetWindowTitle(const std::string& title) {
  window_.setTitle(title);
  return *this;
}

App& App::SetWindowSize(sf::Vector2u size) {
  window_.setSize(size);
  return *this;
}

void App::Run(uint32_t tps, uint32_t fps) {
  tps_ = tps;
  fps_ = fps;

  window_.setFramerateLimit(fps_);

  auto previous = std::chrono::steady_clock::now();
  uint64_t lag = 0;
  while (window_.isOpen()) {
    auto current = std::chrono::steady_clock::now();

    uint64_t elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(current - previous)
            .count();

    previous = current;
    lag += elapsed;

    if (is_scene_unloading_scheduled_) {
      scene_->InternalOnDestroy();
      scene_ = nullptr;
      is_scene_unloading_scheduled_ = false;
    }

    if (scheduled_scene_to_load_) {
      scene_ = std::move(scheduled_scene_to_load_);
      scene_->InternalOnAdd();
      scheduled_scene_to_load_ = nullptr;
    }

    while (lag >= NanosecondsPerTick()) {
      PollInput();
      if (scene_) {
        scene_->InternalUpdate();
      }
      lag -= NanosecondsPerTick();
    }

    window_.clear();

    if (scene_) {
      scene_->InternalDraw(window_);
    }

    window_.display();
  }
}

float App::SecondsPerTick() const {
  return 1.F / static_cast<float>(tps_);
}

uint64_t App::NanosecondsPerTick() const {
  static constexpr uint64_t kNanosInSecond = 1000000000ULL;
  return kNanosInSecond / tps_;
}

const sf::RenderWindow& App::GetWindow() const {
  return window_;
}

ResourceManager& App::GetResourceManager() {
  return resource_manager_;
}

const Input& App::GetInput() const {
  return input_;
}

App& App::LoadScene(std::unique_ptr<Scene> scene) {
  scheduled_scene_to_load_ = std::move(scene);
  return *this;
}

void App::UnloadScene() {
  is_scene_unloading_scheduled_ = true;
}

void App::PollInput() {
  input_.Advance();

  while (std::optional event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window_.close();
    } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
      scene_->OnWindowResize(resized->size);
    }

    input_.Handle(*event);
  }
}

}  // namespace ng