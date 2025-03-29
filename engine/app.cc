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
#include "node.h"

namespace ng {

App& App::GetInstance() {
  static App instance;
  return instance;
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
      scene_ = nullptr;
      is_scene_unloading_scheduled_ = false;
    }

    if (scheduled_scene_to_load_) {
      scene_ = std::move(scheduled_scene_to_load_);
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
      for (const Camera* camera : cameras_) {
        window_.setView(camera->GetView());
        scene_->InternalDraw(*camera, window_);
      }
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

sf::RenderWindow& App::GetMutableWindow() {
  return window_;
}

const Node* App::GetScene() const {
  return scene_.get();
}

App& App::LoadScene(std::unique_ptr<Node> scene) {
  scheduled_scene_to_load_ = std::move(scene);
  return *this;
}

void App::UnloadScene() {
  is_scene_unloading_scheduled_ = true;
}

void App::AddCamera(Camera& camera) {
  cameras_.insert(&camera);
}

void App::RemoveCamera(Camera& camera) {
  cameras_.erase(&camera);
}

void App::AddValidNode(const Node& node) {
  valid_nodes_.insert(&node);
}

void App::RemoveValidNode(const Node& node) {
  valid_nodes_.erase(&node);
}

bool App::IsValid(const Node* node) const {
  return valid_nodes_.contains(node);
}

App::App() {
  static constexpr sf::Vector2u kDefaultWindowSize = {800, 600};
  window_ =
      sf::RenderWindow(sf::VideoMode(kDefaultWindowSize), "Default title");
}

void App::PollInput() {
  Input::GetInstance().Advance();

  while (std::optional event = window_.pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window_.close();
    } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
      for (Camera* camera : cameras_) {
        camera->SetViewSize(sf::Vector2f(resized->size));
      }
    }

    Input::GetInstance().Handle(*event);
  }
}

}  // namespace ng