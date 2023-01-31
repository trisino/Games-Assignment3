#include "../include/Scene.h"

Scene::Scene(GameEngine *game_engine) : m_game(game_engine) {}

Scene::Scene() = default;

void Scene::registerAction(int inputKey, eActionNames actionName) {
  m_actionMap[inputKey] = actionName;
}

int Scene::width() { return m_game->window().getSize().x; }

int Scene::height() { return m_game->window().getSize().y; }

void Scene::drawLine(const Vec2 &p1, const Vec2 &p2) {
  sf::VertexArray line(sf::LinesStrip, 2);

  line[0].position = sf::Vector2f(p1.x, p1.y);
  line[1].position = sf::Vector2f(p2.x, p2.y);

  this->m_game->window().draw(line);
}

const ActionMap &Scene::getActionMap() const { return m_actionMap; }

bool Scene::hasEnded() const { return m_hasEnded; }

size_t Scene::currentFrame() const { return m_currentFrame; }

void Scene::setPaused(bool paused) { m_paused = paused; }

void Scene::simulate(const size_t frames) {}
