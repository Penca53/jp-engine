#include "camera.h"

#include "app.h"

namespace ng {

Camera::Camera() : Camera(0, Layer::kDefault) {}

Camera::Camera(int32_t draw_order, Layer layer)
    : draw_order_(draw_order), layer_(layer) {}

Camera::~Camera() {
  App::GetInstance().RemoveCamera(*this);
}

const sf::View& Camera::GetView() const {
  return view_;
}

sf::View& Camera::GetMutableView() {
  return view_;
}

int32_t Camera::GetDrawOrder() const {
  return draw_order_;
}

Layer Camera::GetLayer() const {
  return layer_;
}

void Camera::Start() {
  view_.setSize(
      {static_cast<float>(App::GetInstance().GetWindow().getSize().x),
       static_cast<float>(App::GetInstance().GetWindow().getSize().y)});
  view_.setCenter(GetGlobalTransform().getPosition());
  App::GetInstance().AddCamera(*this);
}

void Camera::Update() {
  view_.setCenter(GetGlobalTransform().getPosition());
}

}  // namespace ng