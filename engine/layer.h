#pragma once

#include <cstdint>

namespace ng {

enum class Layer : uint64_t {  // NOLINT
  kDefault = 1ULL << 0ULL,
  kUI = 1ULL << 1ULL,
};

}  // namespace ng
