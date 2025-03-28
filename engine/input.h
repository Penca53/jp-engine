#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <bitset>

namespace ng {

// Manages keyboard input for the game.
class Input {
 public:
  // Returns the singleton instance of the App.
  static Input& GetInstance();

  Input(const Input& other) = delete;
  Input& operator=(const Input& other) = delete;
  Input(Input&& other) = delete;
  Input& operator=(Input&& other) = delete;

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
  Input() = default;
  ~Input() = default;

  // The current state of all the keys.
  std::bitset<sf::Keyboard::ScancodeCount> key_states_;
  // The previous (old) state of all the keys.
  std::bitset<sf::Keyboard::ScancodeCount> old_key_states_;
};

// Global functions for input, using the singleton instance, provided for
// convenience.

// Returns true if a key was just pressed, false otherwise.
bool GetKeyDown(sf::Keyboard::Scancode key);
// Returns true if a key is currently held down, false otherwise.
bool GetKey(sf::Keyboard::Scancode key);
// Returns true if a key was just released, false otherwise.
bool GetKeyUp(sf::Keyboard::Scancode key);

}  // namespace ng
