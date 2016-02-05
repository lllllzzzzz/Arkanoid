#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <SFML\Graphics.hpp>
//#include <SFML\Audio.hpp>
#include <unordered_map>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    sf::Texture& GetTexture(const std::string filename);
    sf::Font& GetFont(const std::string filename);
    //sf::Texture& GetTile(const std::string filename);
    //sf::SoundBuffer& GetSound(const std::string filename);

private:
    std::string m_resourcesDir;

    static ResourceManager* _instance;

    std::unordered_map<std::string, sf::Texture*> m_textures;
    //std::mutex TexturesMutex;
    std::unordered_map<std::string, sf::Font*> m_fonts;
    //std::mutex FontsMutex;
    //std::unordered_map<std::string, sf::Texture*> m_tiles;
    //std::mutex TilesMutex;
};

#endif // RESOURCE_MANAGER_HPP
