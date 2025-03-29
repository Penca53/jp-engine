#include "camera.h"

#include "app.h"
#include "layer.h"

#include <cstdint>

#include <SFML/Graphics/View.hpp>

namespace ng {

Camera::Camera() : Camera(0, Layer::kDefault) {}

Camera::Camera(int32_t draw_order, Layer render_layer)
    : draw_order_(draw_order), render_layer_(render_layer) {}

Camera::~Camera() {
  App::GetInstance().RemoveCamera(*this);
}

const sf::View& Camera::GetView() const {
  return view_;
}

int32_t Camera::GetDrawOrder() const {
  return draw_order_;
}

Layer Camera::GetRenderLayer() const {
  return render_layer_;
}

void Camera::SetViewSize(sf::Vector2f size) {
  view_.setSize(size);
}

void Camera::Start() {
  SetViewSize(sf::Vector2f(App::GetInstance().GetWindow().getSize()));
  view_.setCenter(GetGlobalTransform().getPosition());
  App::GetInstance().AddCamera(*this);
}

void Camera::Update() {
  view_.setCenter(GetGlobalTransform().getPosition());
}

}  // namespace ng