#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <cstdint>
#include <memory>
#include <set>
#include <unordered_set>

#include "camera.h"
#include "node.h"

namespace ng {

static constexpr auto cmp_ascending_draw_order = [](const Camera* a,
                                                    const Camera* b) {
  if (a->GetDrawOrder() < b->GetDrawOrder()) {
    return true;
  }
  if (a->GetDrawOrder() > b->GetDrawOrder()) {
    return false;
  }

  return a < b;
};

// The App class is a singleton that manages the (single, main) window and the game loop.
// It handles the rendering of a single scene at a time and the window events.
// It is responsible for creating and destroying the window, as well as
// managing the active cameras and the scene.
class App {
 public:
  // Returns the singleton instance of the App.
  static App& GetInstance();

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
  float SecondsPerTick() const;
  // Returns the number of nanoseconds in a "tick".
  uint64_t NanosecondsPerTick() const;

  // Returns the window.
  const sf::RenderWindow& GetWindow() const;
  // Returns the mutable window.
  sf::RenderWindow& GetMutableWindow();

  // Returns the loaded scene.
  const Node* GetScene() const;

  // Loads a new scene. The old scene will be swapped
  // at the beginning of the next game loop iteration.
  App& LoadScene(std::unique_ptr<Node> scene);
  // Unloads the current scene. The scene will be unloaded
  // at the beginning of the next game loop iteration.
  void UnloadScene();

  // Adds the camera to a set of enabled cameras.
  // If the camera already exists, it is a no-op.
  void AddCamera(Camera& camera);
  // Removes the camera from the set of enabled cameras.
  // If the camera wasn't present, it is a no-op.
  void RemoveCamera(Camera& camera);

  // Adds the node to a set of valid nodes.
  // If the node already exists, it is a no-op.
  void AddValidNode(const Node& node);
  // Removes the node from the set of valid nodes.
  // If the node wasn't present, it is a no-op.
  void RemoveValidNode(const Node& node);
  // Returns true when a node is registered in the
  // valid nodes set.
  // This method should be used whenever the lifetime
  // of a pointer to a node is unsure.
  bool IsValid(const Node* node) const;

 private:
  App();
  ~App() = default;

  void PollInput();

  // Ticks Per Second (update/tick).
  uint32_t tps_ = 0;
  // Frames Per Second (draw/render).
  uint32_t fps_ = 0;

  // The one and only active window.
  sf::RenderWindow window_;

  // Note: The cameras must be declared before scene because of destruction order.
  // The cameras in the scene will unregister in their destructor,
  // so their destructor must be called before the destructor of the cameras field).
  // Contains all the active cameras in the loaded scene.
  std::set<Camera*, decltype(cmp_ascending_draw_order)> cameras_;
  // Contains all the valid, registered nodes in the entire application.
  std::unordered_set<const Node*> valid_nodes_;

  // The one and only loaded scene.
  std::unique_ptr<Node> scene_;

  // The scene enqueued to be loaded next tick.
  std::unique_ptr<Node> scheduled_scene_to_load_;
  // Whether we need to unload the current scene next tick.
  bool is_scene_unloading_scheduled_ = false;
};

}  // namespace ng