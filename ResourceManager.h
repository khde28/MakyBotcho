#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <memory>
#include <iostream>

class ResourceManager {
private:
    std::map<std::string, std::unique_ptr<sf::Texture>> textures;
    std::map<std::string, std::unique_ptr<sf::SoundBuffer>> soundBuffers;
    std::map<std::string, std::unique_ptr<sf::Font>> fonts;
    
    static std::unique_ptr<ResourceManager> instance;

public:
    static ResourceManager& getInstance();
    
    // Cargar recursos
    bool loadTexture(const std::string& name, const std::string& filepath);
    bool loadSound(const std::string& name, const std::string& filepath);
    bool loadFont(const std::string& name, const std::string& filepath);
    
    // Obtener recursos
    const sf::Texture& getTexture(const std::string& name) const;
    const sf::SoundBuffer& getSoundBuffer(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;
    
    // Verificar si existe un recurso
    bool hasTexture(const std::string& name) const;
    bool hasSound(const std::string& name) const;
    bool hasFont(const std::string& name) const;
    
    // Cargar todos los recursos del juego
    bool loadAllResources();
    
    // Limpiar recursos
    void cleanup();
    
private:
    ResourceManager() = default;
};

// Definiciones inline para métodos simples
inline bool ResourceManager::hasTexture(const std::string& name) const {
    return textures.find(name) != textures.end();
}

inline bool ResourceManager::hasSound(const std::string& name) const {
    return soundBuffers.find(name) != soundBuffers.end();
}

inline bool ResourceManager::hasFont(const std::string& name) const {
    return fonts.find(name) != fonts.end();
}