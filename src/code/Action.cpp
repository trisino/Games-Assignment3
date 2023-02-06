#include "../include/Action.h"

Action::Action() = default;

Action::Action(eActionNames name, const eActionTypes &type)
    : m_name(name), m_type(type) {}

eActionNames Action::name() const { return m_name; }

eActionTypes Action::type() const { return m_type; }
