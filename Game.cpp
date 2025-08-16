#include "Game.h"
#include "Config.h"
#include "ResourceManager.h"
#include "Robot.h"
#include "GameLogic.h"
#include <iostream>
#include <fstream>


// Implementación de GameSettings
void GameSettings::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "No se pudo cargar configuración desde: " << filename << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        if (line.find("fullscreen=") == 0) {
            fullscreen = (line.substr(11) == "true");
        } else if (line.find("masterVolume=") == 0) {
            masterVolume = std::stof(line.substr(13));
        } else if (line.find("sfxVolume=") == 0) {
            sfxVolume = std::stof(line.substr(10));
        } else if (line.find("musicVolume=") == 0) {
            musicVolume = std::stof(line.substr(12));
        } else if (line.find("vsync=") == 0) {
            vsync = (line.substr(6) == "true");
        } else if (line.find("targetFPS=") == 0) {
            targetFPS = std::stoi(line.substr(10));
        }
    }
    file.close();
}

void GameSettings::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "No se pudo guardar configuración en: " << filename << std::endl;
        return;
    }
    
    file << "fullscreen=" << (fullscreen ? "true" : "false") << "\n";
    file << "masterVolume=" << masterVolume << "\n";
    file << "sfxVolume=" << sfxVolume << "\n";
    file << "musicVolume=" << musicVolume << "\n";
    file << "vsync=" << (vsync ? "true" : "false") << "\n";
    file << "targetFPS=" << targetFPS << "\n";
    
    file.close();
}

// Implementación de Game
Game::Game() 
    : window()
    , deltaTimeClock()
    , fpsClock()
    , settings()
    , isRunning(true)
    , isInitialized(false)
    , accumulator(0.0f)
    , frameCount(0)
    , fps(0.0f)
{
    // Los unique_ptr se inicializan automáticamente como nullptr
}

Game::~Game() {
    cleanup();
}

bool Game::initialize() {
    if (isInitialized) {
        return true;
    }
    
    std::cout << "Inicializando Makibot v" << Version::VERSION_STRING << std::endl;
    
    // Cargar configuración
    settings.loadFromFile(Paths::SETTINGS_FILE);
    
    // Inicializar subsistemas
    if (!initializeWindow()) {
        std::cerr << "Error: No se pudo inicializar la ventana" << std::endl;
        return false;
    }
    
    if (!loadResources()) {
        std::cerr << "Error: No se pudieron cargar los recursos" << std::endl;
        return false;
    }
    
    if (!initializeManagers()) {
        std::cerr << "Error: No se pudieron inicializar los managers" << std::endl;
        return false;
    }
    
    if (!initializeComponents()) {
        std::cerr << "Error: No se pudieron inicializar los componentes" << std::endl;
        return false;
    }
    
    setupCallbacks();
    logSystemInfo();
    
    isInitialized = true;
    std::cout << "Makibot inicializado correctamente" << std::endl;
    
    return true;
}

void Game::cleanup() {
    if (!isInitialized) {
        return;
    }
    
    // Guardar configuración
    settings.saveToFile(Paths::SETTINGS_FILE);
    
    // Limpiar componentes
    robot.reset();
    gameLogic.reset();
    
    // Limpiar managers
    audioManager.reset();
    levelManager.reset();
    uiManager.reset();
    stateManager.reset();
    
    // Limpiar recursos
    ResourceManager::getInstance().cleanup();
    
    // Cerrar ventana
    if (window.isOpen()) {
        window.close();
    }
    
    isInitialized = false;
    std::cout << "Game cleanup completado" << std::endl;
}

void Game::run() {
    if (!isInitialized) {
        std::cerr << "Error: El juego no está inicializado" << std::endl;
        return;
    }
    
    deltaTimeClock.restart();
    fpsClock.restart();
    
    const float fixedTimeStep = 1.0f / settings.targetFPS;
    
    while (isRunning && window.isOpen()) {
        float deltaTime = deltaTimeClock.restart().asSeconds();
        
        // Limitar deltaTime para evitar saltos grandes
        deltaTime = Utils::clamp(deltaTime, 0.0f, 0.1f);
        
        accumulator += deltaTime;
        
        handleEvents();
        
        // Fixed timestep update
        while (accumulator >= fixedTimeStep) {
            update(fixedTimeStep);
            accumulator -= fixedTimeStep;
        }
        
        render();
        updateFPS(deltaTime);
    }
}

void Game::update(float deltaTime) {
    // Actualizar managers
    if (stateManager) {
        stateManager->update(deltaTime);
    }
    
    if (uiManager) {
        uiManager->update(deltaTime);
    }
    
    if (levelManager) {
        levelManager->update(deltaTime);
    }
    
    if (audioManager) {
        audioManager->update(deltaTime);
    }
    
    // Actualizar componentes principales
    if (robot) {
        robot->update(deltaTime);
    }
    
    if (gameLogic) {
        gameLogic->update(deltaTime);
    }
}

void Game::render() {
    window.clear(GameConfig::BACKGROUND_COLOR);
    
    // Renderizar componentes del juego
    if (levelManager) {
        // levelManager->render(window);
    }
    
    if (robot) {
        // Crear transformación isométrica
        sf::Transform isoTransform;
        // robot->render(window, isoTransform);
    }
    
    // Renderizar UI
    if (uiManager) {
        // uiManager->render(window);
    }
    
    // Renderizar información de debug
    if (Debug::SHOW_FPS) {
        renderDebugInfo();
    }
    
    window.display();
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        handleWindowEvents(event);
        
        // Pasar eventos a managers
        if (uiManager) {
            // uiManager->handleEvent(event);
        }
        
        if (stateManager) {
            // stateManager->handleEvent(event);
        }
    }
}

// Control del juego
void Game::startLevel() {
    if (gameLogic) {
        gameLogic->startExecution();
    }
    std::cout << "Nivel iniciado" << std::endl;
}

void Game::pauseGame() {
    if (gameLogic) {
        gameLogic->pauseExecution();
    }
    std::cout << "Juego pausado" << std::endl;
}

void Game::resumeGame() {
    if (gameLogic) {
        gameLogic->resumeExecution();
    }
    std::cout << "Juego reanudado" << std::endl;
}

void Game::resetLevel() {
    if (gameLogic) {
        gameLogic->resetExecution();
    }
    if (robot) {
        robot->resetToInitialPosition();
    }
    std::cout << "Nivel reiniciado" << std::endl;
}

void Game::resetAllProgress() {
    resetLevel();
    if (levelManager) {
        // levelManager->resetProgress();
    }
    std::cout << "Todo el progreso reiniciado" << std::endl;
}

void Game::nextLevel() {
    if (levelManager) {
        // levelManager->nextLevel();
    }
    resetLevel();
    std::cout << "Siguiente nivel" << std::endl;
}

void Game::goToMenu() {
    if (stateManager) {
        // stateManager->changeState(StateType::MENU);
    }
    std::cout << "Volviendo al menú" << std::endl;
}

void Game::exitGame() {
    isRunning = false;
    std::cout << "Cerrando juego..." << std::endl;
}

// Setters
void Game::setSettings(const GameSettings& newSettings) {
    settings = newSettings;
    applySettings();
}

void Game::applySettings() {
    // Aplicar configuración de ventana
    if (settings.vsync) {
        window.setVerticalSyncEnabled(true);
    } else {
        window.setVerticalSyncEnabled(false);
        window.setFramerateLimit(settings.targetFPS);
    }
    
    // Aplicar configuración de audio
    if (audioManager) {
        // audioManager->setMasterVolume(settings.masterVolume);
        // audioManager->setSFXVolume(settings.sfxVolume);
        // audioManager->setMusicVolume(settings.musicVolume);
    }
}

// Callbacks para eventos del juego
void Game::onLevelComplete() {
    std::cout << "¡Nivel completado!" << std::endl;
    if (audioManager) {
        // audioManager->playSound("level_complete");
    }
    
    // Mostrar UI de nivel completado
    if (uiManager) {
        // uiManager->showLevelCompleteDialog();
    }
}

void Game::onGameOver() {
    std::cout << "Game Over" << std::endl;
    if (audioManager) {
        // audioManager->playSound("game_over");
    }
    
    // Mostrar UI de game over
    if (uiManager) {
        // uiManager->showGameOverDialog();
    }
}

void Game::onRobotCollision() {
    std::cout << "¡Robot colisionó!" << std::endl;
    if (audioManager) {
        // audioManager->playSound("collision");
    }
    
    // Detener ejecución
    if (gameLogic) {
        gameLogic->stopExecution();
    }
}

void Game::onInstructionExecuted() {
    // Actualizar UI de instrucciones
    if (uiManager) {
        // uiManager->updateInstructionHighlight();
    }
}

// Métodos privados de inicialización
bool Game::initializeWindow() {
    sf::VideoMode videoMode(Window::DEFAULT_WIDTH, Window::DEFAULT_HEIGHT);
    
    sf::Uint32 style = sf::Style::Titlebar | sf::Style::Close;
    if (settings.fullscreen) {
        style = sf::Style::Fullscreen;
        videoMode = sf::VideoMode::getDesktopMode();
    }
    
    window.create(videoMode, Window::TITLE, style);
    
    if (!window.isOpen()) {
        return false;
    }
    
    window.setKeyRepeatEnabled(false);
    applySettings();
    
    return true;
}

bool Game::initializeManagers() {
    // Inicializar managers (por ahora solo stubs)
    // stateManager = std::make_unique<StateManager>();
    // uiManager = std::make_unique<UIManager>();
    // levelManager = std::make_unique<LevelManager>();
    // audioManager = std::make_unique<AudioManager>();
    
    return true;
}

bool Game::initializeComponents() {
    // Inicializar robot
    robot = std::make_unique<Robot>(
        World::TILE_SIZE,
        World::ISO_X,
        World::ISO_Y,
        sf::Vector2f(World::ISO_ORIGIN_X, World::ISO_ORIGIN_Y)
    );
    
    if (robot) {
        robot->initialize();
    }
    
    // Inicializar lógica del juego
    gameLogic = std::make_unique<GameLogic>();
    if (gameLogic && robot) {
        // gameLogic->initialize(robot.get(), nullptr); // levelManager->getCurrentLevel()
    }
    
    return true;
}

bool Game::loadResources() {
    return ResourceManager::getInstance().loadAllResources();
}

void Game::setupCallbacks() {
    // Configurar callbacks entre componentes
    // Esto se haría típicamente con señales/slots o funciones de callback
}

// Métodos auxiliares
void Game::updateFPS(float deltaTime) {
    frameCount++;
    
    if (fpsClock.getElapsedTime().asSeconds() >= 1.0f) {
        fps = frameCount / fpsClock.restart().asSeconds();
        frameCount = 0;
        
        updateWindowTitle();
    }
}

void Game::handleWindowEvents(const sf::Event& event) {
    switch (event.type) {
        case sf::Event::Closed:
            exitGame();
            break;
            
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::F11) {
                toggleFullscreen();
            } else if (event.key.code == sf::Keyboard::Escape) {
                goToMenu();
            }
            break;
            
        case sf::Event::Resized:
            // Actualizar viewport
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
            break;
            
        default:
            break;
    }
}

void Game::toggleFullscreen() {
    settings.fullscreen = !settings.fullscreen;
    
    // Recrear ventana
    window.close();
    initializeWindow();
}

void Game::updateWindowTitle() {
    std::string title = Window::TITLE;
    if (Debug::SHOW_FPS) {
        title += " - FPS: " + std::to_string(static_cast<int>(fps));
    }
    window.setTitle(title);
}

void Game::renderDebugInfo() {
    // Renderizar información de debug como FPS, etc.
    // Esto requeriría configurar un sf::Text con una fuente cargada
}

void Game::logSystemInfo() {
    std::cout << "=== Información del Sistema ===" << std::endl;
    std::cout << "Versión SFML: " << SFML_VERSION_MAJOR << "." 
              << SFML_VERSION_MINOR << "." << SFML_VERSION_PATCH << std::endl;
    std::cout << "Resolución de ventana: " << window.getSize().x 
              << "x" << window.getSize().y << std::endl;
    std::cout << "VSync: " << (settings.vsync ? "Activado" : "Desactivado") << std::endl;
    std::cout << "FPS objetivo: " << settings.targetFPS << std::endl;
    std::cout << "==============================" << std::endl;
}

// Implementación de GameUtils
namespace GameUtils {
    sf::Vector2f screenToWorld(const sf::Vector2i& screenPos, const sf::RenderWindow& window) {
        return window.mapPixelToCoords(screenPos);
    }
    
    sf::Vector2i worldToScreen(const sf::Vector2f& worldPos, const sf::RenderWindow& window) {
        return window.mapCoordsToPixel(worldPos);
    }
    
    float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }
    
    float clamp(float value, float min, float max) {
        return value < min ? min : (value > max ? max : value);
    }
    
    sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t) {
        return sf::Color(
            static_cast<sf::Uint8>(a.r + t * (b.r - a.r)),
            static_cast<sf::Uint8>(a.g + t * (b.g - a.g)),
            static_cast<sf::Uint8>(a.b + t * (b.b - a.b)),
            static_cast<sf::Uint8>(a.a + t * (b.a - a.a))
        );
    }
}