#pragma once

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "layer.h"
#include "node.h"

namespace ng {

// Represents a camera in the game world, controlling the viewport,
// draw order and rendering layer.
class Camera : public Node {
 public:
  // Creates a default Camera (draw order 0 and default render layer).
  explicit Camera(App* app);
  // Constructs a Camera with the specified draw order and rendering layer.
  Camera(App* app, int32_t draw_order, Layer layers);

  // Returns the camera's view.
  [[nodiscard]] const sf::View& GetView() const;
  // Returns the camera's draw order.
  [[nodiscard]] int32_t GetDrawOrder() const;
  // Returns the camera's render layer.
  [[nodiscard]] Layer GetRenderLayers() const;
  // Sets the camera's view size.
  void SetViewSize(sf::Vector2f size);

 protected:
  void OnAdd() override;
  void Update() override;
  void OnDestroy() override;

 private:
  // The camera's view.
  sf::View view_;
  // The camera's draw order.
  // Lower values get drawn first.
  int32_t draw_order_ = 0;
  // The camera's render layer.
  // The layer acts as a bitmask, meaning that a camera can render multiple
  // layers at once.
  Layer render_layers_ = Layer::kDefault;
};

}  // namespace ng