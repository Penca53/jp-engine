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

class ResourceManager {
 public:
  static ResourceManager& GetInstance();

  ResourceManager(const ResourceManager& other) = delete;
  ResourceManager& operator=(const ResourceManager& other) = delete;
  ResourceManager(ResourceManager&& other) = delete;
  ResourceManager& operator=(ResourceManager&& other) = delete;

  sf::Font& LoadFont(const std::filesystem::path& filename);
  sf::Texture& LoadTexture(const std::filesystem::path& filename);
  sf::SoundBuffer& LoadSoundBuffer(const std::filesystem::path& filename);

 private:
  ResourceManager() = default;
  ~ResourceManager() = default;

  static constexpr std::string_view kPrefix_ = "resources/";
  std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts_;
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures_;
  std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>>
      sound_buffers_;
};

}  // namespace ng