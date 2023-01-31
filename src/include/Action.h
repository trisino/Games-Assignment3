#pragma once
#include <string>

enum eActionTypes {
  END = 0,
  START
};

enum eActionNames {
  TOGGLE_TEXTURE = 0,
  TOGGLE_COLLISION,
  TOGGLE_GRID,
  PAUSE,
  QUIT,
  UP,
  RIGHT,
  FIRE,
  LEFT,
};

class Action {
  eActionNames m_name;
  eActionTypes m_type;

  public:
  Action();
  Action(eActionNames name, const eActionTypes &type);

  eActionNames name() const;
  eActionTypes type() const;
};
