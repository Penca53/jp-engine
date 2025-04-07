#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <bitset>

namespace ng {

// Manages keyboard input.
class Input {
 public:
  // Updates the key states for the next frame.
  void Advance();
  // Handles SFML window events.
  // Non-Keyboard related event's will result in no-op.
  void Handle(const sf::Event& event);

  // Returns true if a key was just pressed, false otherwise.
  [[nodiscard]] bool GetKeyDown(sf::Keyboard::Scancode key) const;
  // Returns true if a key is currently held down, false otherwise.
  [[nodiscard]] bool GetKey(sf::Keyboard::Scancode key) const;
  // Returns true if a key was just released, false otherwise.
  [[nodiscard]] bool GetKeyUp(sf::Keyboard::Scancode key) const;

 private:
  // The current state of all the keys.
  std::bitset<sf::Keyboard::ScancodeCount> key_states_;
  // The previous (old) state of all the keys.
  std::bitset<sf::Keyboard::ScancodeCount> old_key_states_;
};

}  // namespace ng
