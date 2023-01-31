#include "../include/Scene_Play.h"
#include "../include/Action.h"
#include "../include/Assets.h"
#include "../include/Components.h"
#include "../include/GameEngine.h"
#include <iostream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
  init(m_levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
  registerAction(sf::Keyboard::Z, eActionNames::PAUSE);
  registerAction(sf::Keyboard::Escape, eActionNames::QUIT);
  registerAction(sf::Keyboard::T, eActionNames::TOGGLE_TEXTURE);
  registerAction(sf::Keyboard::C, eActionNames::TOGGLE_COLLISION);
  registerAction(sf::Keyboard::P, eActionNames::TOGGLE_GRID);
  registerAction(sf::Keyboard::Space, eActionNames::UP);
  registerAction(sf::Keyboard::K, eActionNames::RIGHT);
  registerAction(sf::Keyboard::J, eActionNames::LEFT);
  registerAction(sf::Keyboard::D, eActionNames::FIRE);
  //  TODO: Register all other gameplay Actions

  m_gridText.setCharacterSize(12);
  m_gridText.setFont(m_game->getAssets().getFont(eFontTypes::NUMBERS));

  loadLevel(levelPath);
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY,
                                std::shared_ptr<Entity> entity) {
  //      TODO: This function takes in a grid (x,y) position and an Entity
  //            Return a Vec2 indicating where the CENTER position of the Entity
  //            should be You must use the Entity's Animation size to position
  //            it correctly The size of the grid width and height is stored in
  //            m_gridSize.x and m_gridSize.y The up-left corner of the
  //            Animation should align with the bottom left of the grid cell
  float x, y;
  x = gridX + entity->getComponent<CAnimation>().animation.getSize().x / 2;
  y = gridY - entity->getComponent<CAnimation>().animation.getSize().y / 2;
  return Vec2(x, y);
}

void Scene_Play::loadLevel(const std::string &filename) {
  //  reset the entity manager every time we load a level
  m_entityManager = EntityManager();
  // TODO : read in the level file and add the appropriate entities
  //        use the PlayerConfig struct m_playerConfig to store player
  //        proprieties this struct is defined at the top of Scene_Play.h

  // NOTE : all the code below is a sample code which shows you how to
  //        set up and use entities with the new syntax, it should be removed
  std::ifstream file(filename);
  std::string str;
  int Intstr;
  eLevelEntities level_entities;
  eAnimationTypes animation_types;
  while (file.good() && str != " ") {
    file >> str;
    Intstr = std::stoi(str);
    level_entities = static_cast<eLevelEntities>(Intstr);
    if (level_entities < eLevelEntities::LAST && level_entities >= 0) {
      switch (level_entities) {
        case PLAYER:
          file >> m_playerConfig.X >> m_playerConfig.Y >> m_playerConfig.CX >>
              m_playerConfig.CY >> m_playerConfig.JUMP >> m_playerConfig.SPEED >>
              m_playerConfig.MAXHEIGHT >>
              m_playerConfig.GRAVITY;// remain to implement an attribute which is
                                     // the bullet animation
          break;                     // that has to be read from file
        case GOOMBA:
          file >> m_enemyConfig.X >> m_enemyConfig.Y >> m_enemyConfig.SPEED;
          spawnEnemy();// read from the struct that will be overwritten every a new enemy is added
        default:
          int x, y;
          file >> x >> y;
          auto entity = m_entityManager.addEntity(
              (level_entities % 2 == 0) ? DEC : TILE, level_entities);
          animation_types = static_cast<eAnimationTypes>(level_entities);
          entity->addComponents<CAnimation>(
              m_game->getAssets().getAnimation(animation_types), true);
          entity->addComponents<CTransform>(
              gridToMidPixel((float) x * 64, (float) y * 64, entity));
          entity->getComponent<CTransform>().prevPos =
              entity->getComponent<CTransform>().pos;
          if (entity->id() == TILE)
            entity->addComponents<CBoundingBox>(
                entity->getComponent<CAnimation>()
                    .animation
                    .getSize());// has to change have to adapt a correct bounding
                                // box for all the entities'
          //        entity->getComponent<CTransform>().scale.y =
          //        entity->getComponent<CBoundingBox>().size.y /
          //        entity->getComponent<CAnimation>().animation.getSize().y;
          //        entity->getComponent<CTransform>().scale.x =
          //        entity->getComponent<CBoundingBox>().size.x /
          //        entity->getComponent<CAnimation>().animation.getSize().x;
          break;
      }
    } else {
      std::cerr << "unknown entity" << std::endl;
      getline(file, str);// go to the next line for the saving the safety of
                         // the other attributes
    }
  }
  spawnPlayer();
  /*
    // some sample entities
    auto brick = m_entityManager.addEntity("tile");
    // IMPORTANT : always add the CAnimation component first so that
    gridToMidPixel can compute correctly
    brick->addComponents<CAnimation>(m_game->getAssets().getAnimation("Brick"),
    true); brick->addComponents<CTransform>(Vec2(96, 480));
    // NOTE : you finally code should position the entity with the grid x,y
    position read from the file
    //  brick->addComponents<CTransform>(gridToMidPixel(gridX, gridY, brick))

    if (brick->getComponent<CAnimation>().animation.getName() == "Brick")
      std::cout << "This could be a good way of identifying if a tile is a
    brick!" << std::endl;

  */
  //  auto brick = m_entityManager.addEntity(eLevelEntities::BRICK);
  //  brick->addComponents<CAnimation>(m_game->getAssets().getAnimation(eAnimationTypes::AniBRICK),
  //  true); brick->addComponents<CTransform>(Vec2(224, 480));
  //  // add a bounding box, this will now show up if we press the 'C' key
  //  brick->addComponents<CBoundingBox>(m_gridSize);
  //	brick->getComponent<CTransform>().scale.y =
  // brick->getComponent<CBoundingBox>().size.y /
  // brick->getComponent<CAnimation>().animation.getSize().y;
  //	brick->getComponent<CTransform>().scale.x =
  // brick->getComponent<CBoundingBox>().size.x /
  // brick->getComponent<CAnimation>().animation.getSize().x;
  //
  //  auto question = m_entityManager.addEntity("tile");
  //  question->addComponents<CAnimation>(m_game->getAssets().getAnimation("Question"),
  //  true); question->addComponents<CTransform>(Vec2(352, 480));

  // NOTE : THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
  //        Components are now returned as reference rather than pointers
  //        if you do not specify a reference variable type, it will COPY the
  //        component here is an example
  //
  //        This will COPY the transform into the variable 'transform1' - it's
  //        INCORRECT any changes you make to transform1 will not be changed
  //        inside the entity auto transform1 = entity->get<CTransform>();
  //
  //        This will REFERENCE the transform with the variable 'transform2' -
  //        it's CORRECT Now any changes you make to transform2 will be changed
  //        inside the entity auto & transform2 = entity->get<CTransform>()
}

void Scene_Play::spawnPlayer() {
  // here is a sample player entity which you can use to construct other
  // entities

  m_player = m_entityManager.addEntity(eEntitiesType::DYNAMIC, eLevelEntities::PLAYER);
  m_player->addComponents<CGravity>().gravity = m_playerConfig.GRAVITY;
  m_player->addComponents<CAnimation>(
      m_game->getAssets().getAnimation(eAnimationTypes::STAND), true);
  m_player->addComponents<CTransform>(
      gridToMidPixel(64 * m_playerConfig.X, 64 * m_playerConfig.Y, m_player));
  m_player->addComponents<CBoundingBox>(
      Vec2(m_playerConfig.CX, m_playerConfig.CY));
  m_player->getComponent<CTransform>().scale.y =
      64 / m_player->getComponent<CAnimation>().animation.getSize().y;
  m_player->getComponent<CTransform>().scale.x =
      64 / m_player->getComponent<CAnimation>().animation.getSize().x;
  // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawnBullet(const std::shared_ptr<Entity> &entity) {
  Vec2 entityPos = entity->getComponent<CTransform>().pos;
  (entity->getComponent<CTransform>().scale.x < 0) ? entityPos.x -= 64
                                                   : entityPos.x += 64;
  // TODO: this should spawn a bullet at the given entity, going in the
  // direction the entity is facing
  auto bullet = m_entityManager.addEntity(TILE, eLevelEntities::BULLET);
  bullet->addComponents<CAnimation>(
      m_game->getAssets().getAnimation(eAnimationTypes::BUSTER), true);
  bullet->addComponents<CTransform>(
      gridToMidPixel((entityPos.x), entityPos.y, bullet));
  bullet->addComponents<CBoundingBox>(
      bullet->getComponent<CAnimation>().animation.getSize());
  bullet->getComponent<CTransform>().velocity.x =
      entity->getComponent<CTransform>().scale.x * m_playerConfig.SPEED * 6;
  bullet->addComponents<CLifeSpan>(1.8231);
}

void Scene_Play::spawnEnemy() {
  // TODO(due: Friday): implement all the spawn enemy stuff
  // outh use the goomba animation and be added as Tile entity
  // see the current implementation of how the goomba is built
  // form the loadFromFile method
}

void Scene_Play::update() {
  m_entityManager.update();
  // TODO: implement pause functionality
  sMovement();
  sCollision();
  sLifespan();
  sAnimation();
  sRender();
}

void Scene_Play::sMovement() {
  // TODO: implement player movement / jumping based on its CInput component
  // TODO: implement gravity's effect on the player
  // TODO: implement the maximum player speed in both X and Y directions
  // NOTE: setting an  entity's scale.x to -1/1 will make it face to the
  // left/right
  Vec2 vel = {m_playerConfig.SPEED,
              m_player->getComponent<CTransform>().velocity.y};
  // sample example of player movement, you should iterate in the entities
  if (!(m_player->getComponent<CInput>().up ||
        m_player->getComponent<CInput>().left ||
        m_player->getComponent<CInput>().right ||
        m_player->getComponent<CInput>().shoot)) {
    m_player->addComponents<CAnimation>(
        m_game->getAssets().getAnimation(eAnimationTypes::STAND), true);
    m_player->getComponent<CState>().state = STANDING;
  }

  while (true) {
    if (m_player->getComponent<CInput>().up) {
      m_player->getComponent<CTransform>().velocity.y = -(m_playerConfig.JUMP);
      if (m_player->getComponent<CTransform>().pos.y <=
          m_player->getComponent<CTransform>().aimPos) {
        m_player->getComponent<CInput>().up = false;
        m_player->getComponent<CTransform>().velocity.y = 0;
        m_player->getComponent<CState>().notGrounded = true;
      }
      break;
    }
    if (m_player->getComponent<CInput>().left) {
      m_player->getComponent<CTransform>().velocity.x = -(vel.x);
      break;
    } else
      m_player->getComponent<CTransform>().velocity.x = 0;

    if (m_player->getComponent<CInput>().right)
      m_player->getComponent<CTransform>().velocity.x = vel.x;
    else
      m_player->getComponent<CTransform>().velocity.x = 0;
    break;
  }

  // m_player->getComponent<CTransform>().prevPos =
  //     m_player->getComponent<CTransform>().pos;

  for (auto &e: m_entityManager.getEntities()) {
    if (e->hasComponent<CTransform>()) {
      e->getComponent<CTransform>().prevPos =
          e->getComponent<CTransform>().pos;
      if (e->hasComponent<CGravity>() && e->getComponent<CState>().notGrounded) {
        e->getComponent<CTransform>().velocity.y +=
            e->getComponent<CGravity>().gravity;
        e->getComponent<CTransform>().pos.y +=
            e->getComponent<CTransform>().velocity.y;
      }
      e->getComponent<CTransform>().pos.x +=
          e->getComponent<CTransform>().velocity.x;
    }
  }

  // add spawnEnemy()
  if (m_player->getComponent<CTransform>().pos.y > height())
    spawnPlayer();
}

void Scene_Play::sLifespan() {
  // TODO: check lifespan of entities that have them, and destroy them if they
  // go over
  for (auto e: m_entityManager.getEntities()) {
    if (e->hasComponent<CLifeSpan>())
      if (e->getComponent<CLifeSpan>().lifespan <
          e->getComponent<CLifeSpan>().clock.getElapsedTime().asSeconds())
        e->destroy();
  }
}

void Scene_Play::sCollision() {
  // REMEMBER: SFML's (0,0) position is in the TOP-Left corner
  //           this means jumping will have negative y-component
  //           and gravity will have positive y-component
  //           Also, something BELOW something else will have a y value GREATER
  //           than it Also, something ABOVE something else will have a y value
  //           LOWER than it
  // TODO: implement Physics::GetOverlap() function, use it inside this function
  // TODO: implement bullet / tile collisions
  //       destroy the tile if it has a Brick animation
  // TODO: implement player / tile collisions and resolutions
  //       update the CState component of the player to store whether
  //       used by the Animation system
  // this is temporary cause we could optimize everything using in some way
  // only one cycle, try to do that
  Vec2 overlapRect;
  for (auto &dynamicEntity: m_entityManager.getEntities(DYNAMIC)) {
    dynamicEntity->getComponent<CState>().notGrounded = true;
    for (auto &tileEntity: m_entityManager.getEntities(TILE)) {
      overlapRect = m_physics.GetOverlap(tileEntity, dynamicEntity);
      if (dynamicEntity->getComponent<CTransform>().pos.x <= 32)
        dynamicEntity->getComponent<CTransform>().pos.x += 1;

      if (overlapRect > 0) {
        switch (m_physics.GetPreviousOverlap(tileEntity, dynamicEntity).x > 0) {
          case 1:
            if (dynamicEntity->getComponent<CTransform>().pos.y <
                dynamicEntity->getComponent<CTransform>().prevPos.y) {
              switch (tileEntity->tag()) {
                case BRICK:
                  tileEntity->addComponents<CAnimation>(
                      m_game->getAssets().getAnimation(eAnimationTypes::EXPLOSION),
                      false);
                  tileEntity->getComponent<CBoundingBox>().size = {0, 0};
                  dynamicEntity->getComponent<CInput>().up = false;
                  dynamicEntity->getComponent<CTransform>().velocity.y =
                      dynamicEntity->getComponent<CGravity>().gravity;
                  break;

                default:
                  break;
              }
            } else {
              //          if (dynamicEntity->getComponent<CState>().state == STANDING) {
              //          // the comment will disable the double jump
              dynamicEntity->getComponent<CInput>().canJump = true;
              dynamicEntity->getComponent<CState>().notGrounded = false;
              dynamicEntity->getComponent<CTransform>().velocity.y = 0;
              //          }
              //          dynamicEntity->getComponent<CState>().state = STANDING;
            }
            dynamicEntity->getComponent<CTransform>().pos.y -= overlapRect.y;
            break;
          default:
            dynamicEntity->getComponent<CTransform>().pos.x -=
                (dynamicEntity->getComponent<CTransform>().scale.x /
                 std::abs(dynamicEntity->getComponent<CTransform>().scale.x)) *
                overlapRect.x;
            break;
        }
      }
    }
    // this is the starting point, then you'll have to handle the various types of
    // collisions with a switch case perhaps
  }
}

void Scene_Play::sDoAction(const Action &action) {
  if (action.type() == eActionTypes::START) {
    for (auto &e: m_entityManager.getEntities(DYNAMIC)) {
      if (e->getComponent<CInput>().canJump) {
        e->getComponent<CInput>().down = false;
        e->getComponent<CInput>().left = false;
        e->getComponent<CInput>().right = false;
        e->getComponent<CInput>().shoot = false;
      }
    }
    switch (action.name()) {
      case eActionNames::TOGGLE_TEXTURE: {
        m_drawTextures = !m_drawTextures;
        break;
      }
      case eActionNames::TOGGLE_COLLISION: {
        m_drawCollision = !m_drawCollision;
        break;
      }
      case eActionNames::TOGGLE_GRID: {
        m_drawGrid = !m_drawGrid;
        break;
      }
      case eActionNames::PAUSE: {
        setPaused(!m_paused);
        break;
      }

      case eActionNames::QUIT: {
        onEnd();
        break;
      }
      case eActionNames::UP: {
        if (m_player->getComponent<CInput>().canJump) {
          m_player->getComponent<CInput>().up = true;
          m_player->getComponent<CState>().state = JUMPING;
          m_player->addComponents<CAnimation>(
              m_game->getAssets().getAnimation(eAnimationTypes::JUMP), true);
          m_player->getComponent<CTransform>().aimPos =
              m_player->getComponent<CTransform>().pos.y -
              m_playerConfig.MAXHEIGHT;
          m_player->getComponent<CInput>().canJump = false;
          m_player->getComponent<CState>().notGrounded = true;
        }
        break;
      }
      case eActionNames::RIGHT:
        m_player->getComponent<CInput>().right = true;
        m_player->addComponents<CAnimation>(
            m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
        m_player->getComponent<CState>().state = RUNNING;
        (m_player->getComponent<CTransform>().scale.x < 0)
            ? m_player->getComponent<CTransform>().scale.x *= -1
            : 1;
        break;
      case eActionNames::LEFT:
        m_player->getComponent<CInput>().left = true;
        m_player->addComponents<CAnimation>(
            m_game->getAssets().getAnimation(eAnimationTypes::RUN), true);
        (m_player->getComponent<CTransform>().scale.x > 0)
            ? m_player->getComponent<CTransform>().scale.x *= -1
            : 1;
        m_player->getComponent<CState>().state = RUNNING;
        break;
      case FIRE:
        m_player->getComponent<CInput>().shoot = true;
        m_player->addComponents<CAnimation>(
            m_game->getAssets().getAnimation(eAnimationTypes::SHOT), true);
        spawnBullet(m_player);
        m_player->getComponent<CState>().state = SHOUTING;
        break;
    }
  } else if (action.type() == eActionTypes::END) {
    // has to be fixed the movement bug
    switch (action.name()) {
      case eActionNames::RIGHT:
        m_player->getComponent<CInput>().right = false;
        break;
      case eActionNames::LEFT:
        m_player->getComponent<CInput>().left = false;
        break;
      case eActionNames::UP:
        if (m_player->getComponent<CInput>().up) {
          m_player->getComponent<CInput>().up = false;
          m_player->getComponent<CTransform>().velocity.y = 0;
          m_player->getComponent<CState>().notGrounded = true;
        }
        break;
      case eActionNames::FIRE:
        m_player->getComponent<CInput>().shoot = false;
        m_player->getComponent<CState>().state = STANDING;
        break;
    }
  }
}

void Scene_Play::sAnimation() {
  // TODO: complete the Animation class code first

  // TODO: set the animation of the player based on its CState component
  // TODO: for each entity with an animation, call
  // entity->getComponent<CAnimation>().animation.update();
  //       if the animation is not repeated, and it has ended, destroy the
  //       entity
  for (auto &e: m_entityManager.getEntities()) {
    if (e->hasComponent<CAnimation>()) {
      e->getComponent<CAnimation>().animation.update();
      if (e->getComponent<CAnimation>().animation.hasEnded() &&
          !e->getComponent<CAnimation>().repeat)
        e->destroy();
    }
  }
}

void Scene_Play::onEnd() {
  // TODO: when the scene ends, change back to the MENU scene
  //       use m_game->changeState(correct params)
  m_game->changeScene(MENU, m_game->getScene(MENU));
}

void Scene_Play::sRender() {
  // color the background darker, so you know that the game is paused
  if (!m_paused) {
    m_game->window().clear(sf::Color(87, 174, 209));
  } else {
    m_game->window().clear(sf::Color(50, 50, 150));
  }

  // set the viewport of the window to be centered on the player if its far
  // enough right
  auto &pPos = m_player->getComponent<CTransform>().pos;
  float windowCenterX = std::max(m_game->window().getSize().x / 2.0f, pPos.x);
  sf::View view = m_game->window().getView();
  view.setCenter(windowCenterX,
                 m_game->window().getSize().y - view.getCenter().y);
  m_game->window().setView(view);

  // draw all Entity textures / animations
  if (m_drawTextures) {
    for (auto &e: m_entityManager.getEntities()) {
      auto &transform = e->getComponent<CTransform>();
      if (e->hasComponent<CAnimation>()) {
        auto &animation = e->getComponent<CAnimation>().animation;
        animation.getSprite().setRotation(transform.angle);
        animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
        animation.getSprite().setScale(transform.scale.x, transform.scale.y);
        m_game->window().draw(animation.getSprite());
      }
    }
  }

  // draw all Entity collision bounding boxes with a rectangle-shape
  if (m_drawCollision) {
    for (auto &e: m_entityManager.getEntities()) {
      if (e->hasComponent<CBoundingBox>()) {
        auto &box = e->getComponent<CBoundingBox>();
        auto &transform = e->getComponent<CTransform>();
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
        rect.setOrigin(sf::Vector2f(box.size.x / 2, box.size.y / 2));
        rect.setPosition(transform.pos.x, transform.pos.y);
        rect.setFillColor(sf::Color(0, 0, 0, 0));
        rect.setOutlineColor(sf::Color(255, 255, 255, 255));
        rect.setOutlineThickness(1);
        m_game->window().draw(rect);
      }
    }
  }

  // draw gri
  if (m_drawGrid) {
    for (float x = m_gridSize.x; x <= width(); x += m_gridSize.x)
      drawLine(Vec2(x, 0), Vec2(x, height()));

    for (float y = 0; y < height(); y += m_gridSize.y) {
      drawLine(Vec2(0, height() - y), Vec2(width(), height() - y));

      for (int i = 0; i < width(); i += m_gridSize.x) {
        for (int j = 0; j < height(); j += m_gridSize.y) {
          m_gridText.setPosition(i, j);
          m_gridText.setCharacterSize(12);
          m_gridText.setString("(" + std::to_string((int) i / 64) + "," +
                               std::to_string((int) j / 64) + ")");
          ;
          m_game->window().draw(m_gridText);
        }
      }
    }
  }
  m_game->window().display();
}
