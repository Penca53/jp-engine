#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>

namespace ng {

class Input {
 public:
  static Input& GetInstance();

  Input(const Input& other) = delete;
  Input& operator=(const Input& other) = delete;
  Input(Input&& other) = delete;
  Input& operator=(Input&& other) = delete;

 private:
  Input() = default;
  ~Input() = default;

 public:
  void Advance();

  void Handle(const sf::Event& event);

  [[nodiscard]] bool GetKeyDown(sf::Keyboard::Scancode key) const;
  [[nodiscard]] bool GetKey(sf::Keyboard::Scancode key) const;
  [[nodiscard]] bool GetKeyUp(sf::Keyboard::Scancode key) const;

 private:
  std::bitset<sf::Keyboard::ScancodeCount> key_states_;
  std::bitset<sf::Keyboard::ScancodeCount> old_key_states_;
};

bool GetKeyDown(sf::Keyboard::Scancode key);
bool GetKey(sf::Keyboard::Scancode key);
bool GetKeyUp(sf::Keyboard::Scancode key);

}  // namespace ng
