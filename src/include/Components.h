#pragma once
#include "Animation.h"
#include "Assets.h"

enum eComponentStates {
  JUMPING = 0,
  FALLING,
  RUNNING,
  STANDING,
  SHOUTING
};

class Components {
  public:
  bool has = false;
};

class CTransform : public Components {
  public:
  Vec2 pos = {0.0, 0.0};
  Vec2 prevPos = {0.0, 0.0};
  Vec2 scale = {1.0, 1.0};
  Vec2 velocity = {0.0, 0.0};
  float angle = 0;
  float aimPos = 0;

  CTransform() = default;
  explicit CTransform(const Vec2 &p)
      : pos(p) {}
  CTransform(const Vec2 &p, const Vec2 &sp, const Vec2 &sc, float a)
      : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

class CBoundingBox : public Components {
  public:
  Vec2 size = {0.0, 0.0};

  CBoundingBox() = default;
  explicit CBoundingBox(const Vec2 &s)
      : size(s) {}
};

class CLifeSpan : public Components {
  public:
  sf::Clock clock{};
  float lifespan = 0;

  explicit CLifeSpan(int l) : lifespan(l) { clock.restart(); }
};

class CInput : public Components {
  public:
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool shoot = false;
  bool canShoot = true;
  bool canJump = true;
  CInput() = default;
};

class CAnimation : public Components {
  public:
  CAnimation() = default;
  Animation animation;
  bool repeat;

  CAnimation(const Animation &animation, bool r)
      : animation(animation), repeat(r) {}
};

class CGravity : public Components {
  public:
  CGravity() {}
  float gravity;
  int knockback;
  CGravity(float g, int kb) : gravity(g), knockback(kb) {}
};

class CState : public Components {
  public:
  CState() = default;
  eComponentStates state;

  size_t frames = 0;
  bool notGrounded = true;
  explicit CState(eComponentStates &s) : state(s) {}
};
