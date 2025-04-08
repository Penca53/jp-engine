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
  // Accumulator for unprocessed time.
  uint64_t lag = 0;
  while (window_.isOpen()) {
    auto current = std::chrono::steady_clock::now();

    // Calculate elapsed time since the last frame.
    uint64_t elapsed =
        std::chrono::duration_cast<std::chrono::nanoseconds>(current - previous)
            .count();

    previous = current;
    lag += elapsed;

    // Unload the current scene if it's scheduled.
    if (is_scene_unloading_scheduled_) {
      // Notify the scene that it's being destroyed.
      scene_->InternalOnDestroy();
      // Release ownership of the current scene.
      scene_ = nullptr;
      is_scene_unloading_scheduled_ = false;
    }

    // Load the scheduled scene if there is one.
    if (scheduled_scene_to_load_) {
      // Move the scheduled scene to the active scene. Transferring ownership.
      scene_ = std::move(scheduled_scene_to_load_);
      // Notify the new scene that it has been added.
      scene_->InternalOnAdd();
      // Reset the scheduled scene.
      scheduled_scene_to_load_ = nullptr;
    }

    // Process game logic updates based on the target TPS.
    while (lag >= NanosecondsPerTick()) {
      PollInput();
      if (scene_) {
        scene_->InternalUpdate();
      }
      lag -= NanosecondsPerTick();
    }

    window_.clear();

    // Draw the current scene if it exists.
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
  // Schedule the new scene to be loaded in the next frame.
  // Ownership of 'scene' is moved to 'scheduled_scene_to_load_'.
  scheduled_scene_to_load_ = std::move(scene);
  return *this;
}

void App::UnloadScene() {
  // Set the flag to unload the current scene in the next frame.
  is_scene_unloading_scheduled_ = true;
}

void App::PollInput() {
  // Prepare the input handler for new events.
  input_.Advance();

  // Poll all pending SFML events.
  while (std::optional event = window_.pollEvent()) {
    // Handle window closing event.
    if (event->is<sf::Event::Closed>()) {
      window_.close();
    } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
      // Notify the current scene about window resizing.
      if (scene_) {
        scene_->OnWindowResize(resized->size);
      }
    }

    // Pass the event to the input handler for processing.
    input_.Handle(*event);
  }
}

}  // namespace ng