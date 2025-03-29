#pragma once

#include <SFML/Graphics/View.hpp>
#include <cstdint>

#include "layer.h"
#include "node.h"

namespace ng {

// Represents a camera in the game world, controlling the viewport,
// draw order and rendering layer.
class Camera : public Node {
 public:
  // Creates a default Camera (draw order 0 and default render layer).
  Camera();
  // Constructs a Camera with the specified draw order and rendering layer.
  Camera(int32_t draw_order, Layer layer);
  ~Camera() override;

  Camera(const Camera& other) = delete;
  Camera& operator=(const Camera& other) = delete;
  Camera(Camera&& other) = delete;
  Camera& operator=(Camera&& other) = delete;

  // Returns the camera's view.
  const sf::View& GetView() const;
  // Returns the camera's draw order.
  int32_t GetDrawOrder() const;
  // Returns the camera's render layer.
  Layer GetRenderLayer() const;
  // Sets the camera's view size.
  void SetViewSize(sf::Vector2f size);

 protected:
  void Start() override;
  void Update() override;

 private:
  // The camera's view.
  sf::View view_;
  // The camera's draw order.
  // Lower values get drawn first.
  int32_t draw_order_ = 0;
  // The camera's render layer.
  // The layer acts as a bitmask, meaning that a camera can render multiple
  // layers at once.
  Layer render_layer_ = Layer::kDefault;
};

}  // namespace ng