#include "collider.h"

#include "app.h"

namespace ng {

Collider::~Collider() {
  App::GetInstance().GetMutablePhysics().RemoveCollider(*this);
}

void Collider::Start() {
  App::GetInstance().GetMutablePhysics().AddCollider(*this);
}

}  // namespace ng