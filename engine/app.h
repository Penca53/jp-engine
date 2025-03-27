#pragma once

#include <SFML/Graphics.hpp>
#include <set>
#include <unordered_set>

#include "camera.h"
#include "node.h"

namespace ng {

static constexpr auto cmp_ascending_draw_order = [](const Camera* a,
                                                    const Camera* b) {
  if (a->GetDrawOrder() < b->GetDrawOrder()) {
    return true;
  } else if (a->GetDrawOrder() > b->GetDrawOrder()) {
    return false;
  }

  return a < b;
};

class App {
 public:
  static App& GetInstance();

  App& SetWindowTitle(const std::string& title);
  App& SetWindowSize(sf::Vector2u size);

  void Run(uint32_t tps, uint32_t fps);
  float SecondsPerTick() const;
  uint64_t NanosecondsPerTick() const;

  const sf::RenderWindow& GetWindow() const;
  sf::RenderWindow& GetMutableWindow();

  const Node* GetScene() const;

  App& LoadScene(std::unique_ptr<Node> scene);
  void UnloadScene();

  void AddCamera(Camera& camera);
  void RemoveCamera(Camera& camera);

  void AddValidNode(const Node& node);
  void RemoveValidNode(const Node& node);

  bool IsValid(const Node* node) const;

 private:
  App();
  void PollInput();

 private:
  uint32_t tps_ = 0;
  uint32_t fps_ = 0;
  sf::RenderWindow window_;

  // The cameras must be declared before scene because of destruction order.
  // The cameras in the scene will unregister in their destructor,
  // so their destructor must be called before the destructor of the cameras field).
  std::set<Camera*, decltype(cmp_ascending_draw_order)> cameras_;
  std::unordered_set<const Node*> valid_nodes_;

  std::unique_ptr<Node> scene_;
  std::unique_ptr<Node> scheduled_scene_to_load_;
  bool is_scene_unloading_scheduled_ = false;
};

}  // namespace ng