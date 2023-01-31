#include "../include/Assets.h"
#include <cassert>

Assets::Assets() = default;

void Assets::loadFromFile(const std::string &path) {
  std::ifstream file(path);
  std::string str;
  eAnimationTypes animation_types;
  eTextureTypes texture_types;
  eFontTypes font_types;
  file >> str;
  while (file.good() && str != " ") {
    switch (toascii(str[0])) {
    case eAssetsTypes::ANIMATION: {
      std::string name, texture;
      size_t frames, speed;
      file >> name >> texture >> frames >> speed;
      animation_types = static_cast<eAnimationTypes>(std::stoi(name));
      texture_types = static_cast<eTextureTypes>(std::stoi(texture));

      if (animation_types < eAnimationTypes::ALAST &&
          texture_types < eTextureTypes::TLAST && animation_types > 0)
        addAnimation(animation_types, texture_types, frames, speed);
      else {
        std::cerr << "unknown animation or his texture enumeration"
                  << std::endl;
        getline(file, str);
      }

      break;
    }
    case eAssetsTypes::TEXTURE: {
      std::string nameT, pathT;
      file >> nameT >> pathT;
      texture_types = static_cast<eTextureTypes>(std::stoi(nameT));

      if (texture_types < eTextureTypes::TLAST && texture_types > 0)
        addTexture(texture_types, pathT);

      else {
        std::cerr << "unknown texture enumeration" << std::endl;
        getline(file, str);
      }

      break;
    }
    case eAssetsTypes::FONT: {
      std::string name, path;
      file >> name >> path;
      font_types = static_cast<eFontTypes>(std::stoi(name));

      if (font_types < eFontTypes::FLAST && font_types > 0)
        addFont(font_types, path);

      else {
        std::cerr << "unknown font enumeration " << std::endl;
        getline(file, str);
      }

      break;
    }
    default: {
      std::cerr << "unknown Asset Type: " << str << std::endl;
      getline(file, str);
      break;
    }
    }
    file >> str;
  }
}

void Assets::addTexture(eTextureTypes &texture_types, const std::string &path,
                        bool smooth) {
  m_textureMap[texture_types] = sf::Texture();
  if (!m_textureMap[texture_types].loadFromFile(path)) {
    std::cerr << "Could not load texture file: " << path << std::endl;
    m_textureMap.erase(texture_types);
  } else {
    m_textureMap[texture_types].setSmooth(smooth);
    std::cout << "Loaded Texture: " << path << std::endl;
  }
}

const sf::Texture &Assets::getTexture(eTextureTypes texture_types) const {
  assert(m_textureMap.find(texture_types) != m_textureMap.end());
  return m_textureMap.at(texture_types);
}

void Assets::addAnimation(eAnimationTypes &animation_types,
                          eTextureTypes &texture_types, size_t frameCount,
                          size_t speed) {
  //  m_animation_typess.push_back(animation_types);
  m_animationMap[animation_types] =
      Animation(animation_types, getTexture(texture_types), frameCount, speed);
}

const Animation &Assets::getAnimation(eAnimationTypes animation_types) const {
  assert(m_animationMap.find(animation_types) != m_animationMap.end());
  return m_animationMap.at(animation_types);
}

// const std::vector<std::string> & Assets::getanimation_typess() const
//{
//   return m_animation_types;
// }

void Assets::addFont(eFontTypes &fontName, const std::string &path) {
  m_fontMap[fontName] = sf::Font();
  if (!m_fontMap[fontName].loadFromFile(path)) {
    std::cerr << "Could not load font file: " << path << std::endl;
    m_fontMap.erase(fontName);
  } else {
    std::cout << "Loaded Font:    " << path << std::endl;
  }
}

const sf::Font &Assets::getFont(eFontTypes fontName) const {
  assert(m_fontMap.find(fontName) != m_fontMap.end());
  return m_fontMap.at(fontName);
}
