#pragma once

#include "layer.h"
#include "node.h"

namespace ng {

class Camera : public Node {
 public:
  Camera();
  Camera(int32_t draw_order, Layer layer);
  virtual ~Camera();

  const sf::View& GetView() const;
  sf::View& GetMutableView();
  int32_t GetDrawOrder() const;
  Layer GetLayer() const;

 protected:
  void Start() override;
  void Update() override;

 private:
  sf::View view_;
  int32_t draw_order_ = 0;
  Layer layer_ = Layer::kDefault;
};

}  // namespace ng