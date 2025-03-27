#include "collider.h"

#include "physics.h"

namespace ng {

Collider::~Collider() {
  Physics::GetInstance().RemoveCollider(*this);
}

void Collider::Start() {
  Physics::GetInstance().AddCollider(*this);
}

}  // namespace ng