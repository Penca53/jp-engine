#pragma once

#include <cstdint>

namespace ng {

// Defines bitmask layers for organizing and filtering nodes.
// It is used by cameras to determine which nodes to render (i.e. UI).
enum class Layer : uint64_t {  // NOLINT
  kDefault = 1ULL << 0ULL,
  kUI = 1ULL << 1ULL,
};

}  // namespace ng
