#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

namespace ng {

class ResourceManager {
 public:
  static ResourceManager& GetInstance();

  ResourceManager(ResourceManager const&) = delete;
  void operator=(ResourceManager const&) = delete;

  sf::Font& LoadFont(const std::filesystem::path& filename);
  sf::Texture& LoadTexture(const std::filesystem::path& filename);
  sf::SoundBuffer& LoadSoundBuffer(const std::filesystem::path& filename);

 private:
  ResourceManager() = default;

 private:
  static constexpr std::string_view kPrefix_ = "resources/";
  std::unordered_map<std::string, std::unique_ptr<sf::Font>> fonts_;
  std::unordered_map<std::string, std::unique_ptr<sf::Texture>> textures_;
  std::unordered_map<std::string, std::unique_ptr<sf::SoundBuffer>>
      sound_buffers_;
};

}  // namespace ng