#pragma once

#include <memory>

#include "engine/scene.h"

namespace game {

std::unique_ptr<ng::Scene> MakeDefaultScene();

}  // namespace game