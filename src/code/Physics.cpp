#include "../include/Physics.h"
#include "../include/Vec2.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
  // TODO: return the overlap rectangle size of the bounding boxes of entity a
  // and b
  Vec2 aPos = {a->getComponent<CTransform>().pos};
  Vec2 bPos = {b->getComponent<CTransform>().pos};
  Vec2 aSize = a->getComponent<CBoundingBox>().size;
  Vec2 bSize = b->getComponent<CBoundingBox>().size;

  Vec2 delta = {abs(aPos.x - bPos.x), abs(aPos.y - bPos.y)};
  float overlapX = (aSize.x / 2 + bSize.x / 2) - delta.x;
  float overlapY = (aSize.y / 2 + bSize.y / 2) - delta.y;

  return {overlapX, overlapY};
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a,
                                 std::shared_ptr<Entity> b) {
  // TODO: return the previous overlap rectangle size of the bounding boxes of
  // entity a and b
  //       previous overlap the entity's previous position
  Vec2 aPos = {a->getComponent<CTransform>().prevPos};
  Vec2 bPos = {b->getComponent<CTransform>().prevPos};
  Vec2 aSize = a->getComponent<CBoundingBox>().size;
  Vec2 bSize = b->getComponent<CBoundingBox>().size;

  Vec2 delta = {abs(aPos.x - bPos.x), abs(aPos.y - bPos.y)};
  float overlapX = (aSize.x / 2 + bSize.x / 2) - delta.x;
  float overlapY = (aSize.y / 2 + bSize.y / 2) - delta.y;

  return {overlapX, overlapY};
}
