#pragma once

#include <SFML/Graphics.hpp>
#include <bitset>

namespace ng {

class Input {
 public:
  static Input& GetInstance();

  Input(Input const&) = delete;
  void operator=(Input const&) = delete;

 private:
  Input() = default;

 public:
  void Advance();

  void Handle(const sf::Event& event);

  bool GetKeyDown(sf::Keyboard::Scancode key) const;
  bool GetKey(sf::Keyboard::Scancode key) const;
  bool GetKeyUp(sf::Keyboard::Scancode key) const;

 protected:
  std::bitset<sf::Keyboard::ScancodeCount> key_states_;
  std::bitset<sf::Keyboard::ScancodeCount> old_key_states_;
};

bool GetKeyDown(sf::Keyboard::Scancode key);
bool GetKey(sf::Keyboard::Scancode key);
bool GetKeyUp(sf::Keyboard::Scancode key);

}  // namespace ng
