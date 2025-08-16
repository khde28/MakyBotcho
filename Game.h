#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <memory>

// Forward declarations
class StateManager;
class ResourceManager;
class UIManager;
class Robot;
class GameLogic;
class LevelManager;
class AudioManager;

// Constantes del juego
namespace GameConfig {
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 750;
    const int TARGET_FPS = 60;
    const std::string WINDOW_TITLE = "Makibot";
    const sf::Color BACKGROUND_COLOR(169, 169, 169);  // Gris oscuro
}

// Configuración del juego
struct GameSettings {
    bool fullscreen = false;
    float masterVolume = 1.0f;
    float sfxVolume = 1.0f;
    float musicVolume = 0.3f;
    bool vsync = true;
    int targetFPS = GameConfig::TARGET_FPS;
    
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;
};

// Clase principal del juego
class Game {
private:
    // Ventana y renderizado
    sf::RenderWindow window;
    sf::Clock deltaTimeClock;
    sf::Clock fpsClock;
    
    // Managers principales
    std::unique_ptr<StateManager> stateManager;
    std::unique_ptr<UIManager> uiManager;
    std::unique_ptr<LevelManager> levelManager;
    std::unique_ptr<AudioManager> audioManager;
    
    // Componentes del juego
    std::unique_ptr<Robot> robot;
    std::unique_ptr<GameLogic> gameLogic;
    
    // Configuración
    GameSettings settings;
    
    // Estado del juego
    bool isRunning = true;
    bool isInitialized = false;
    float accumulator = 0.0f;
    
    // Estadísticas de rendimiento
    int frameCount = 0;
    float fps = 0.0f;
    sf::Text fpsText;

public:
    Game();
    ~Game();
    
    // Inicialización y limpieza
    bool initialize();
    void cleanup();
    
    // Bucle principal
    void run();
    void update(float deltaTime);
    void render();
    void handleEvents();
    
    // Control del juego
    void startLevel();
    void pauseGame();
    void resumeGame();
    void resetLevel();
    void resetAllProgress();
    void nextLevel();
    void goToMenu();
    void exitGame();
    
    // Getters para managers
    StateManager* getStateManager() const { return stateManager.get(); }
    UIManager* getUIManager() const { return uiManager.get(); }
    LevelManager* getLevelManager() const { return levelManager.get(); }
    AudioManager* getAudioManager() const { return audioManager.get(); }
    Robot* getRobot() const { return robot.get(); }
    GameLogic* getGameLogic() const { return gameLogic.get(); }
    
    // Getters de estado
    sf::RenderWindow& getWindow() { return window; }
    const GameSettings& getSettings() const { return settings; }
    bool getIsRunning() const { return isRunning; }
    float getFPS() const { return fps; }
    
    // Setters
    void setSettings(const GameSettings& newSettings);
    void applySettings();
    
    // Callbacks para eventos del juego
    void onLevelComplete();
    void onGameOver();
    void onRobotCollision();
    void onInstructionExecuted();
    
private:
    // Métodos de inicialización
    bool initializeWindow();
    bool initializeManagers();
    bool initializeComponents();
    bool loadResources();
    void setupCallbacks();
    
    // Métodos auxiliares
    void updateFPS(float deltaTime);
    void handleWindowEvents(const sf::Event& event);
    void toggleFullscreen();
    void updateWindowTitle();
    
    // Debug
    void renderDebugInfo();
    void logSystemInfo();
};

// Funciones globales de utilidad
namespace GameUtils {
    sf::Vector2f screenToWorld(const sf::Vector2i& screenPos, const sf::RenderWindow& window);
    sf::Vector2i worldToScreen(const sf::Vector2f& worldPos, const sf::RenderWindow& window);
    float distance(const sf::Vector2f& a, const sf::Vector2f& b);
    float clamp(float value, float min, float max);
    sf::Color lerpColor(const sf::Color& a, const sf::Color& b, float t);
}