#include "resource_manager.h"

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <memory>
#include <utility>

namespace ng {

sf::Texture& ResourceManager::LoadTexture(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path =
      std::filesystem::absolute(kPrefix_ / filename);

  auto it = textures_.find(full_path);
  if (it == textures_.end()) {
    auto texture = std::make_unique<sf::Texture>(full_path);
    it = textures_.insert({full_path, std::move(texture)}).first;
  }

  return *(it->second);
}

sf::SoundBuffer& ResourceManager::LoadSoundBuffer(
    const std::filesystem::path& filename) {
  std::filesystem::path full_path =
      std::filesystem::absolute(kPrefix_ / filename);

  auto it = sound_buffers_.find(full_path);
  if (it == sound_buffers_.end()) {
    auto sound_buffer = std::make_unique<sf::SoundBuffer>(full_path);
    it = sound_buffers_.insert({full_path, std::move(sound_buffer)}).first;
  }

  return *(it->second);
}

sf::Font& ResourceManager::LoadFont(const std::filesystem::path& filename) {
  std::filesystem::path full_path =
      std::filesystem::absolute(kPrefix_ / filename);

  auto it = fonts_.find(full_path);
  if (it == fonts_.end()) {
    auto font = std::make_unique<sf::Font>(full_path);
    it = fonts_.insert({full_path, std::move(font)}).first;
  }

  return *(it->second);
}

}  // namespace ng