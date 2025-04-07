#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>

#include "input.h"
#include "resource_manager.h"
#include "scene.h"

namespace ng {

class App {
 public:
  App(sf::Vector2u window_size, const std::string& window_title);
  ~App() = default;

  App(const App& other) = delete;
  App& operator=(const App& other) = delete;
  App(App&& other) = delete;
  App& operator=(App&& other) = delete;

  // Sets the title of the window.
  App& SetWindowTitle(const std::string& title);
  // Sets the size of the window in pixels.
  App& SetWindowSize(sf::Vector2u size);

  // Runs the game loop. Execution is blocked until
  // the game loop breaks.
  void Run(uint32_t tps, uint32_t fps);

  // Returns the number of seconds in a "tick".
  [[nodiscard]] float SecondsPerTick() const;
  // Returns the number of nanoseconds in a "tick".
  [[nodiscard]] uint64_t NanosecondsPerTick() const;

  // Returns the window.
  [[nodiscard]] const sf::RenderWindow& GetWindow() const;
  [[nodiscard]] ResourceManager& GetResourceManager();
  [[nodiscard]] const Input& GetInput() const;

  // Loads a new scene. The old scene will be swapped
  // at the beginning of the next game loop iteration.
  App& LoadScene(std::unique_ptr<Scene> scene);
  // Unloads the current scene. The scene will be unloaded
  // at the beginning of the next game loop iteration.
  void UnloadScene();

 private:
  void PollInput();

  // Ticks Per Second (update/tick).
  uint32_t tps_ = 0;
  // Frames Per Second (draw/render).
  uint32_t fps_ = 0;

  // The one and only active window.
  sf::RenderWindow window_;

  ResourceManager resource_manager_;
  Input input_;

  // The one and only loaded scene.
  std::unique_ptr<Scene> scene_;
  // The scene enqueued to be loaded next tick.
  std::unique_ptr<Scene> scheduled_scene_to_load_;
  // Whether we need to unload the current scene next tick.
  bool is_scene_unloading_scheduled_ = false;
};

}  // namespace ng