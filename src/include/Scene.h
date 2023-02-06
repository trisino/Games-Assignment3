#pragma once

#include "Action.h"
#include "EntityManager.h"
#include "GameEngine.h"

#include <memory>

typedef std::map<int, eActionNames> ActionMap;

class GameEngine;

class Scene {
protected:
  GameEngine *m_game = nullptr;
  EntityManager m_entityManager;
  ActionMap m_actionMap;
  bool m_paused = false;
  bool m_hasEnded = false;
  size_t m_currentFrame = 0;

  virtual void onEnd() = 0;
  void setPaused(bool paused);

public:
  Scene();
  explicit Scene(GameEngine *game_engine);

  virtual void update() = 0;
  virtual void sDoAction(const Action &action) = 0;
  virtual void sRender() = 0;

  void simulate(const size_t frames);
  void registerAction(int inputKey, eActionNames actionName);

  int width();
  int height();
  size_t currentFrame() const;

  bool hasEnded() const;
  const ActionMap &getActionMap() const;
  void drawLine(const Vec2 &p1, const Vec2 &p2);
};
