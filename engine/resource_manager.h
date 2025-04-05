#pragma once

#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <filesystem>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ng {

// Manages the loading and caching of game resources, such as fonts, textures,
// and sound buffers.
class ResourceManager {
 public:
  ResourceManager() = default;
  ~ResourceManager() = default;

  ResourceManager(const ResourceManager& other) = delete;
  ResourceManager& operator=(const ResourceManager& other) = delete;
  ResourceManager(ResourceManager&& other) = delete;
  ResourceManager& operator=(ResourceManager&& other) = delete;

  // Loads a texture from the specified file.
  // If the resource was already loaded, it returns the cached value.
  // Otherwise, it loads the resource from disk.
  sf::Texture& LoadTexture(const std::filesystem::path& filename);
  // Loads a sound buffer from the specified file.
  // If the resource was already loaded, it returns the cached value.
  // Otherwise, it loads the resource from disk.
  sf::SoundBuffer& LoadSoundBuffer(const std::filesystem::path& filename);
  // Loads a font from the specified file.
  // If the resource was already loaded, it returns the cached value.
  // Otherwise, it loads the resource from disk.
  sf::Font& LoadFont(const std::filesystem::path& filename);

 private:
  // Prefix for resource file paths.
  static constexpr std::string_view kPrefix_ = "resources/";

  // Stores loaded cached textures.
  std::unordered_map<std::filesystem::path, std::unique_ptr<sf::Texture>>
      textures_;
  // Stores loaded cached sound buffers.
  std::unordered_map<std::filesystem::path, std::unique_ptr<sf::SoundBuffer>>
      sound_buffers_;
  // Stores loaded cached fonts.
  std::unordered_map<std::filesystem::path, std::unique_ptr<sf::Font>> fonts_;
};

}  // namespace ng