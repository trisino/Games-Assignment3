#pragma once
#include "Common.h"
#include "Physics.h"
#include "Scene.h"


class Scene_Play : public Scene {
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED, MAXHEIGHT, JUMP, GRAVITY;
  };

  protected:
  std::shared_ptr<Entity> m_player;
  std::string m_levelPath;
  PlayerConfig m_playerConfig;
  bool m_drawTextures = true;
  bool m_drawCollision = false;
  bool m_drawGrid = false;
  const Vec2 m_gridSize = {64, 64};
  sf::Text m_gridText;
  Physics m_physics;

  void init(const std::string &levelPath);
  void loadLevel(const std::string &filename);
  void spawnPlayer();
  void update() override;
  void onEnd() override;

  void spawnBullet(const std::shared_ptr<Entity> &entity);
  void spawnEnemy();

  Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

  void sMovement();
  void sLifespan();
  void sCollision();
  void sDoAction(const Action &action) override;
  void sAnimation();
  void sRender() override;

  public:
  Scene_Play(GameEngine *gameEngine, const std::string &levelPath);
};
