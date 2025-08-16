#include "ResourceManager.h"

std::unique_ptr<ResourceManager> ResourceManager::instance = nullptr;

ResourceManager& ResourceManager::getInstance() {
    if (!instance) {
        instance = std::unique_ptr<ResourceManager>(new ResourceManager());
    }
    return *instance;
}

bool ResourceManager::loadTexture(const std::string& name, const std::string& filepath) {
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filepath)) {
        std::cerr << "Error cargando textura: " << filepath << std::endl;
        return false;
    }
    textures[name] = std::move(texture);
    return true;
}

bool ResourceManager::loadSound(const std::string& name, const std::string& filepath) {
    auto soundBuffer = std::make_unique<sf::SoundBuffer>();
    if (!soundBuffer->loadFromFile(filepath)) {
        std::cerr << "Error cargando sonido: " << filepath << std::endl;
        return false;
    }
    soundBuffers[name] = std::move(soundBuffer);
    return true;
}

bool ResourceManager::loadFont(const std::string& name, const std::string& filepath) {
    auto font = std::make_unique<sf::Font>();
    if (!font->loadFromFile(filepath)) {
        std::cerr << "Error cargando fuente: " << filepath << std::endl;
        return false;
    }
    fonts[name] = std::move(font);
    return true;
}

const sf::Texture& ResourceManager::getTexture(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return *(it->second);
    }
    throw std::runtime_error("Textura no encontrada: " + name);
}

const sf::SoundBuffer& ResourceManager::getSoundBuffer(const std::string& name) const {
    auto it = soundBuffers.find(name);
    if (it != soundBuffers.end()) {
        return *(it->second);
    }
    throw std::runtime_error("Sonido no encontrado: " + name);
}

const sf::Font& ResourceManager::getFont(const std::string& name) const {
    auto it = fonts.find(name);
    if (it != fonts.end()) {
        return *(it->second);
    }
    throw std::runtime_error("Fuente no encontrada: " + name);
}

bool ResourceManager::loadAllResources() {
    bool success = true;
    
    // Cargar texturas
    success &= loadTexture("piso", "images/loza_verde.png");
    success &= loadTexture("piso2d", "images/loza_verde2d.png");
    success &= loadTexture("bloque", "images/loza_naranja.png");
    success &= loadTexture("bloque2d", "images/loza_naranja2d.png");
    success &= loadTexture("loza_azul", "images/loza_azul.png");
    success &= loadTexture("loza_azul2d", "images/loza_azul2d.png");
    success &= loadTexture("sprite_robot", "images/sprite_carritos.png");
    success &= loadTexture("semaforo", "images/semaforo.png");
    
    // Texturas de semáforo
    success &= loadTexture("semaforo_rojo", "images/srojo.png");
    success &= loadTexture("semaforo_naranja", "images/snaranja.png");
    success &= loadTexture("semaforo_verde", "images/sverde.png");
    
    // Texturas de UI
    success &= loadTexture("btn_go", "images/go.png");
    success &= loadTexture("btn_increment", "images/imageincrement.png");
    success &= loadTexture("btn_decrement", "images/imagendecrement.png");
    success &= loadTexture("btn_pausa", "images/pausa.png");
    success &= loadTexture("btn_continua", "images/continua.png");
    success &= loadTexture("btn_nivel1", "images/nivel1.png");
    success &= loadTexture("btn_menu", "images/menu.png");
    success &= loadTexture("btn_restart1", "images/restart_1.png");
    success &= loadTexture("btn_restart_all", "images/restart_a.png");
    success &= loadTexture("btn_if", "images/if.png");
    success &= loadTexture("btn_else", "images/else.png");
    
    // Texturas de botones de comando
    success &= loadTexture("btn_vacio", "images/vacio.png");
    success &= loadTexture("btn_avanzar", "images/avanzar.png");
    success &= loadTexture("btn_giro_derecha", "images/giroright.png");
    success &= loadTexture("btn_giro_izquierda", "images/giroleft.png");
    success &= loadTexture("btn_foco", "images/foco.png");
    success &= loadTexture("btn_f1", "images/f1.png");
    success &= loadTexture("btn_bucle", "images/bucle.png");
    success &= loadTexture("btn_current", "images/current.png");
    
    // Texturas de números
    for (int i = 0; i < 6; ++i) {
        success &= loadTexture("numero_" + std::to_string(i), "images/" + std::to_string(i) + ".png");
    }
    
    // Cargar sonidos
    success &= loadSound("click1", "sounds/click1.ogg");
    success &= loadSound("click2", "sounds/click2.ogg");
    success &= loadSound("mapa1", "sounds/sonido_mapa.ogg");
    success &= loadSound("mapa2", "sounds/sonido_mapa2.ogg");
    success &= loadSound("mapa3", "sounds/sonido_mapa3.ogg");
    
    if (!success) {
        std::cerr << "Error: No se pudieron cargar todos los recursos!" << std::endl;
    }
    
    return success;
}

void ResourceManager::cleanup() {
    textures.clear();
    soundBuffers.clear();
    fonts.clear();
}