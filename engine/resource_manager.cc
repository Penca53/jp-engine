#include "resource_manager.h"

#include <SFML/Audio.hpp>
#include <iostream>

namespace ng {

ResourceManager& ResourceManager::GetInstance() {
  static ResourceManager instance;
  return instance;
}

sf::Font& ResourceManager::LoadFont(const std::filesystem::path& filename) {
  std::filesystem::path full_path = kPrefix_ / filename;
  if (!fonts_.count(full_path)) {
    auto font = std::make_unique<sf::Font>(full_path);
    fonts_.insert({full_path, std::move(font)});
  }

  return *fonts_.at(full_path);
}

sf::Texture& ResourceManager::LoadTexture(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path = kPrefix_ / filename;
  if (!textures_.count(full_path)) {
    auto texture = std::make_unique<sf::Texture>(full_path);
    textures_.insert({full_path, std::move(texture)});
  }

  return *textures_.at(full_path);
}

sf::SoundBuffer& ResourceManager::LoadSoundBuffer(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path = kPrefix_ / filename;
  if (!sound_buffers_.count(full_path)) {
    auto sound_buffer = std::make_unique<sf::SoundBuffer>(full_path);
    sound_buffers_.insert({full_path, std::move(sound_buffer)});
  }

  return *sound_buffers_.at(full_path);
}

}  // namespace ng