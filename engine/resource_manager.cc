#include "resource_manager.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <memory>
#include <utility>

namespace ng {

sf::Font& ResourceManager::LoadFont(const std::filesystem::path& filename) {
  std::filesystem::path full_path = kPrefix_ / filename;

  auto it = fonts_.find(full_path);
  if (it != fonts_.end()) {
    auto font = std::make_unique<sf::Font>(full_path);
    it = fonts_.insert({full_path, std::move(font)}).first;
  }

  return *(it->second);
}

sf::Texture& ResourceManager::LoadTexture(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path = kPrefix_ / filename;
  if (!textures_.contains(full_path)) {
    auto texture = std::make_unique<sf::Texture>(full_path);
    textures_.insert({full_path, std::move(texture)});
  }

  return *textures_.at(full_path);
}

sf::SoundBuffer& ResourceManager::LoadSoundBuffer(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path = kPrefix_ / filename;
  if (!sound_buffers_.contains(full_path)) {
    auto sound_buffer = std::make_unique<sf::SoundBuffer>(full_path);
    sound_buffers_.insert({full_path, std::move(sound_buffer)});
  }

  return *sound_buffers_.at(full_path);
}

}  // namespace ng