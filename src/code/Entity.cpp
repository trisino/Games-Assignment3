#include "../include/Entity.h"

Entity::Entity(eEntitiesType &id, eLevelEntities &entityTag)
    : m_tag(entityTag), m_id(id) {
}
Entity::Entity() = default;
bool Entity::isActive() const {
  return m_active;
}

void Entity::destroy() {
  m_active = false;
}

eEntitiesType Entity::id() const {
  return m_id;
}

eLevelEntities Entity::tag() const {
  return m_tag;
}
