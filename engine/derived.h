#pragma once

#include <type_traits>

namespace ng {

// Enforces T to be derived from U.
template <class T, class U>
concept Derived = std::is_base_of_v<U, T>;

}  // namespace ng