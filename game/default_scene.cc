#include "default_scene.h"

#include <cstdint>
#include <memory>
#include <utility>
#include "background.h"
#include "banana.h"
#include "end.h"
#include "engine/camera.h"
#include "engine/layer.h"
#include "engine/node.h"
#include "engine/resource_manager.h"
#include "engine/tile.h"
#include "engine/tilemap.h"
#include "engine/tileset.h"
#include "follow_player.h"
#include "game_manager.h"
#include "goomba.h"
#include "mario.h"
#include "plant.h"
#include "score_manager.h"
#include "tile_id.h"

namespace game {

std::unique_ptr<ng::Node> MakeDefaultScene() {
  auto scene = std::make_unique<ng::Node>();
  scene->SetName("Scene");
  scene->SetLayer(
      static_cast<ng::Layer>(std::to_underlying(ng::Layer::kDefault) |
                             std::to_underlying(ng::Layer::kUI)));

  ng::Tileset tileset({32, 32}, ng::ResourceManager::GetInstance().LoadTexture(
                                    "Terrain (16x16).png"));

  {
    tileset.AddTile(ng::Tile(TileID::kVoid));
    tileset.AddTile(ng::Tile(TileID::kInvisibleBarrier));

    tileset.AddTile(ng::Tile(TileID::kDirtTopLeft,
                             sf::IntRect({6 * 16, 0 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtTopCenter,
                             sf::IntRect({7 * 16, 0 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtTopRight,
                             sf::IntRect({8 * 16, 0 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtMiddleLeft,
                             sf::IntRect({6 * 16, 1 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtMiddleCenter,
                             sf::IntRect({7 * 16, 1 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtMiddleRight,
                             sf::IntRect({8 * 16, 1 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtBottomLeft,
                             sf::IntRect({6 * 16, 2 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtBottomCenter,
                             sf::IntRect({7 * 16, 2 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kDirtBottomRight,
                             sf::IntRect({8 * 16, 2 * 16}, {16, 16})));

    tileset.AddTile(ng::Tile(TileID::kStoneHorizontalLeft,
                             sf::IntRect({12 * 16, 4 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kStoneHorizontalCenter,
                             sf::IntRect({13 * 16, 4 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kStoneHorizontalRight,
                             sf::IntRect({14 * 16, 4 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kStoneVerticalTop,
                             sf::IntRect({15 * 16, 4 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kStoneVerticalMiddle,
                             sf::IntRect({15 * 16, 5 * 16}, {16, 16})));
    tileset.AddTile(ng::Tile(TileID::kStoneVerticalBottom,
                             sf::IntRect({15 * 16, 6 * 16}, {16, 16})));

    tileset.AddTile(ng::Tile(TileID::kPlasticBlock,
                             sf::IntRect({12 * 16, 9 * 16}, {16, 16})));
  }

  std::unique_ptr<ng::Tilemap> tilemap =
      std::make_unique<ng::Tilemap>(sf::Vector2u(64, 32), tileset);

  for (uint32_t i = 0; i < tilemap->GetSize().x; ++i) {
    tilemap->SetTile({i, 0}, TileID::kStoneHorizontalCenter);
  }

  for (uint32_t i = 1; i < tilemap->GetSize().x; ++i) {
    tilemap->SetTile({i, tilemap->GetSize().y - 3}, TileID::kDirtTopCenter);
    tilemap->SetTile({i, tilemap->GetSize().y - 2}, TileID::kDirtMiddleCenter);
    tilemap->SetTile({i, tilemap->GetSize().y - 1}, TileID::kDirtMiddleCenter);
  }

  for (uint32_t i = 0; i < tilemap->GetSize().y; ++i) {
    tilemap->SetTile({0, i}, TileID::kStoneVerticalMiddle);
    tilemap->SetTile({tilemap->GetSize().x - 1, i},
                     TileID::kStoneVerticalMiddle);
  }

  tilemap->SetTile({13, 27}, TileID::kDirtTopLeft);
  tilemap->SetTile({14, 27}, TileID::kDirtTopRight);
  tilemap->SetTile({13, 28}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({14, 28}, TileID::kDirtMiddleRight);

  tilemap->SetTile({23, 26}, TileID::kDirtTopLeft);
  tilemap->SetTile({24, 26}, TileID::kDirtTopRight);
  tilemap->SetTile({23, 27}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({24, 27}, TileID::kDirtMiddleRight);
  tilemap->SetTile({23, 28}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({24, 28}, TileID::kDirtMiddleRight);

  tilemap->SetTile({26, 29}, TileID::kVoid);
  tilemap->SetTile({26, 30}, TileID::kVoid);
  tilemap->SetTile({26, 31}, TileID::kVoid);
  tilemap->SetTile({27, 29}, TileID::kVoid);
  tilemap->SetTile({27, 30}, TileID::kVoid);
  tilemap->SetTile({27, 31}, TileID::kVoid);

  tilemap->SetTile({25, 29}, TileID::kDirtTopRight);
  tilemap->SetTile({25, 30}, TileID::kDirtMiddleRight);
  tilemap->SetTile({25, 31}, TileID::kDirtMiddleRight);
  tilemap->SetTile({28, 29}, TileID::kDirtTopLeft);
  tilemap->SetTile({28, 30}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({28, 31}, TileID::kDirtMiddleLeft);

  tilemap->SetTile({32, 28}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({33, 28}, TileID::kDirtMiddleRight);
  tilemap->SetTile({32, 27}, TileID::kDirtTopLeft);
  tilemap->SetTile({33, 27}, TileID::kDirtTopRight);

  tilemap->SetTile({41, 26}, TileID::kDirtTopLeft);
  tilemap->SetTile({42, 26}, TileID::kDirtTopRight);
  tilemap->SetTile({41, 27}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({42, 27}, TileID::kDirtMiddleRight);
  tilemap->SetTile({41, 28}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({42, 28}, TileID::kDirtMiddleRight);

  for (uint32_t i = 41 - 4; i < 41; ++i) {
    tilemap->SetTile({i, 25}, TileID::kPlasticBlock);
  }

  tilemap->SetTile({46, 29}, TileID::kVoid);
  tilemap->SetTile({46, 30}, TileID::kVoid);
  tilemap->SetTile({46, 31}, TileID::kVoid);
  tilemap->SetTile({47, 29}, TileID::kVoid);
  tilemap->SetTile({47, 30}, TileID::kVoid);
  tilemap->SetTile({47, 31}, TileID::kVoid);
  tilemap->SetTile({48, 29}, TileID::kVoid);
  tilemap->SetTile({48, 30}, TileID::kVoid);
  tilemap->SetTile({48, 31}, TileID::kVoid);
  tilemap->SetTile({49, 29}, TileID::kVoid);
  tilemap->SetTile({49, 30}, TileID::kVoid);
  tilemap->SetTile({49, 31}, TileID::kVoid);

  tilemap->SetTile({45, 29}, TileID::kDirtTopRight);
  tilemap->SetTile({45, 30}, TileID::kDirtMiddleRight);
  tilemap->SetTile({45, 31}, TileID::kDirtMiddleRight);
  tilemap->SetTile({50, 29}, TileID::kDirtTopLeft);
  tilemap->SetTile({50, 30}, TileID::kDirtMiddleLeft);
  tilemap->SetTile({50, 31}, TileID::kDirtMiddleLeft);

  tilemap->SetTile({53, 28}, TileID::kDirtTopLeft);
  tilemap->SetTile({54, 28}, TileID::kDirtTopCenter);
  tilemap->SetTile({55, 28}, TileID::kDirtTopCenter);
  tilemap->SetTile({56, 28}, TileID::kDirtTopRight);
  tilemap->SetTile({57, 28}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({58, 28}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({59, 28}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({60, 28}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({61, 28}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({62, 28}, TileID::kDirtMiddleCenter);

  tilemap->SetTile({56, 27}, TileID::kDirtTopLeft);
  tilemap->SetTile({57, 27}, TileID::kDirtTopCenter);
  tilemap->SetTile({58, 27}, TileID::kDirtTopCenter);
  tilemap->SetTile({59, 27}, TileID::kDirtTopRight);
  tilemap->SetTile({60, 27}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({61, 27}, TileID::kDirtMiddleCenter);
  tilemap->SetTile({62, 27}, TileID::kDirtMiddleCenter);

  tilemap->SetTile({59, 26}, TileID::kDirtTopLeft);
  tilemap->SetTile({60, 26}, TileID::kDirtTopCenter);
  tilemap->SetTile({61, 26}, TileID::kDirtTopCenter);
  tilemap->SetTile({62, 26}, TileID::kDirtTopCenter);

  tilemap->SetTile({60, 23}, TileID::kInvisibleBarrier);
  tilemap->SetTile({60, 24}, TileID::kInvisibleBarrier);
  tilemap->SetTile({60, 25}, TileID::kInvisibleBarrier);
  tilemap->SetTile({61, 23}, TileID::kInvisibleBarrier);
  tilemap->SetTile({61, 24}, TileID::kInvisibleBarrier);
  tilemap->SetTile({61, 25}, TileID::kInvisibleBarrier);

  std::unique_ptr<ScoreManager> score_manager =
      std::make_unique<ScoreManager>();

  std::unique_ptr<GameManager> game_manager = std::make_unique<GameManager>();

  std::unique_ptr<Mario> mario =
      std::make_unique<Mario>(*tilemap, *score_manager, *game_manager);
  mario->SetLocalPosition(
      {8.F * static_cast<float>(tilemap->GetTileSize().x),
       25.F * static_cast<float>(tilemap->GetTileSize().y)});

  std::unique_ptr<ng::Camera> ui_camera =
      std::make_unique<ng::Camera>(1, ng::Layer::kUI);

  std::unique_ptr<ng::Camera> camera = std::make_unique<ng::Camera>();
  std::unique_ptr<FollowPlayer> follow_player =
      std::make_unique<FollowPlayer>(*mario);
  camera->AddChild(std::move(follow_player));

  std::unique_ptr<Background> background = std::make_unique<Background>(
      tilemap->GetSize().componentWiseMul(tilemap->GetTileSize()));

  std::unique_ptr<End> end = std::make_unique<End>(*game_manager);
  end->SetLocalPosition({61 * 32, 24 * 32});

  std::unique_ptr<Goomba> g = std::make_unique<Goomba>(*tilemap);
  g->SetLocalPosition({38 * 32, 27 * 32});

  std::unique_ptr<Goomba> g1 = std::make_unique<Goomba>(*tilemap);
  g1->SetLocalPosition({40 * 32, 27 * 32});

  std::unique_ptr<Plant> p = std::make_unique<Plant>(*tilemap);
  p->SetLocalPosition({52 * 32, (28 * 32) - 10});

  std::unique_ptr<Plant> p1 = std::make_unique<Plant>(*tilemap);
  p1->SetLocalPosition({58 * 32, (26 * 32) - 10});

  std::unique_ptr<Banana> b = std::make_unique<Banana>();
  b->SetLocalPosition({14 * 32, 26 * 32});

  std::unique_ptr<Banana> b1 = std::make_unique<Banana>();
  b1->SetLocalPosition({40 * 32, 28 * 32});

  scene->AddChild(std::move(background));
  scene->AddChild(std::move(tilemap));
  scene->AddChild(std::move(ui_camera));
  scene->AddChild(std::move(camera));
  scene->AddChild(std::move(mario));
  scene->AddChild(std::move(end));
  scene->AddChild(std::move(g));
  scene->AddChild(std::move(g1));
  scene->AddChild(std::move(p));
  scene->AddChild(std::move(p1));
  scene->AddChild(std::move(b));
  scene->AddChild(std::move(b1));
  scene->AddChild(std::move(score_manager));
  scene->AddChild(std::move(game_manager));

  return scene;
}

}  // namespace game