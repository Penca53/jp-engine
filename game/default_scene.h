#pragma once

#include <memory>

#include "engine/node.h"

namespace game {

std::unique_ptr<ng::Node> MakeDefaultScene();

}  // namespace game