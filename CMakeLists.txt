cmake_minimum_required(VERSION 3.16)
project(SFML3 VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SFML3 src/code/Entity.cpp src/code/Assets.cpp src/code/Animation.cpp src/code/EntityManager.cpp src/code/GameEngine.cpp src/code/main.cpp src/code/Scene.cpp src/code/Scene_Menu.cpp src/code/Scene_Play.cpp src/code/Vec2.cpp src/code/Action.cpp src/code/Physics.cpp)

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(SFML3 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
