#pragma once

#include <cstdint>

namespace ng {

enum class Layer : uint64_t {
  kDefault = 1ull << 0,
  kUI = 1ull << 1,
};

}  // namespace ng
