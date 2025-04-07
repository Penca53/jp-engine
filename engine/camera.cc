#include "camera.h"

#include <SFML/Graphics/View.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "app.h"
#include "layer.h"
#include "node.h"

namespace ng {

Camera::Camera(App& app) : Camera(app, 0, Layer::kDefault) {}

Camera::Camera(App& app, int32_t draw_order, Layer render_layer)
    : Node(app), draw_order_(draw_order), render_layer_(render_layer) {}

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

void Camera::OnAdd() {
  SetViewSize(sf::Vector2f(GetApp().GetWindow().getSize()));
  view_.setCenter(GetGlobalTransform().getPosition());
  GetScene().GetCameraManager().AddCamera(this);
}

void Camera::Update() {
  view_.setCenter(GetGlobalTransform().getPosition());
}

void Camera::OnDestroy() {
  GetScene().GetCameraManager().RemoveCamera(this);
}

}  // namespace ng