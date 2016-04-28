#include "resourceManager.hpp"

#include <iostream>

const std::string ResourceManager::RESOURCES_DIR = "data\\";
const std::string ResourceManager::TEXTURES_DIR = "gfx\\";
const std::string ResourceManager::SOUNDS_DIR = "se\\";
const std::string ResourceManager::FONTS_DIR = "font\\";

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{

}

sf::Texture& ResourceManager::GetTexture(const std::string& filename)
{
    //std::lock_guard<std::mutex> lock(TexturesMutex);
    auto itr = m_textures.find(filename);
    if (itr != m_textures.end()) {
        return *itr->second;
    }

    sf::Texture* texture = new sf::Texture();
    texture->loadFromFile(RESOURCES_DIR + TEXTURES_DIR + filename);

    m_textures[filename] = texture;
    return *texture;
}

sf::Font& ResourceManager::GetFont(const std::string& filename)
{
    //std::lock_guard<std::mutex> lock(TexturesMutex);
    auto itr = m_fonts.find(filename);
    if (itr != m_fonts.end()) {
        return *itr->second;
    }

    sf::Font* font = new sf::Font();
    font->loadFromFile(RESOURCES_DIR + FONTS_DIR + filename);

    m_fonts[filename] = font;
    return *font;
}

sf::SoundBuffer& ResourceManager::GetSound(const std::string& filename)
{
    //std::lock_guard<std::mutex> lock(TexturesMutex);
    auto itr = m_sounds.find(filename);
    if (itr != m_sounds.end()) {
        //std::cout << "loaded" << std::endl;
        return *itr->second;
    }

    sf::SoundBuffer* sound = new sf::SoundBuffer();
    sound->loadFromFile(RESOURCES_DIR + SOUNDS_DIR + filename);

    m_sounds[filename] = sound;
    return *sound;
}
