
#include "../include/EntityManager.h"
#include "../include/Entity.h"
EntityManager::EntityManager() = default;

void EntityManager::update() {
  // add all the entities that are pending
  for (auto &e: m_entitiesToAdd) {
    // add it to the vector of all entities
    m_entities.push_back(e);

    // add it to the entity map in the correct place
    // map[key] will create an element at 'key' if it does not already exist
    //          therefore we are not in danger of adding to a vector that doesn't exist
    m_entityMap[e->id()].push_back(e);
  }

  // clear the temporary vector since we have added everything
  m_entitiesToAdd.clear();

  // clean up dead entities in all vectors
  removeDeadEntities(m_entities);
  for (auto &kv: m_entityMap) {
    // kv is a key-value pair contained in the map
    //    key   (kv.first):  the tag string
    //    value (kv.second): the vector storing entities
    removeDeadEntities(kv.second);
  }
}

void EntityManager::removeDeadEntities(EntityVec &vec) {
  // use std::remove_if to remove dead entities
  // probably not the fastest solution, but it is safe
  vec.erase(std::remove_if(vec.begin(), vec.end(),
                           [](const std::shared_ptr<Entity> &entity) { return !entity->isActive(); }),
            vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(eEntitiesType entityType, eLevelEntities entityTag) {
  // creat the entity shared pointer
  auto entity = std::shared_ptr<Entity>(new Entity(entityType, entityTag));

  // add it to the vector of entities that will be added on next update() call
  m_entitiesToAdd.push_back(entity);

  // return the shared pointer to the entity
  return entity;
}

EntityVec &EntityManager::getEntities() {
  return m_entities;
}

EntityVec &EntityManager::getEntities(eEntitiesType type) {
  // return the vector in the map where all the entities with the same tag live
  return m_entityMap[type];
}
