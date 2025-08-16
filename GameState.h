#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

// Forward declarations
class Game;

// Estados del juego
enum class GameStateType {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER,
    LEVEL_COMPLETE
};

// Clase base para estados
class GameState {
public:
    virtual ~GameState() = default;
    
    virtual void enter() = 0;
    virtual void exit() = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void handleInput(const sf::Event& event) = 0;
    
protected:
    Game* game = nullptr;
    
public:
    void setGame(Game* gamePtr) { game = gamePtr; }
};

// Estado del menú principal
class MenuState : public GameState {
private:
    sf::Sprite menuSprite;
    sf::Sprite continuaSprite;
    sf::Sprite nivel1Sprite;
    
public:
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;
};

// Estado de juego activo
class PlayingState : public GameState {
private:
    class GameData* gameData;
    
public:
    PlayingState();
    ~PlayingState();
    
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;
};

// Estado de pausa
class PausedState : public GameState {
private:
    sf::RectangleShape overlay;
    sf::Sprite menuSprite;
    sf::Sprite continuaSprite;
    sf::Sprite nivel1Sprite;
    
public:
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;
};

// Estado de nivel completado
class LevelCompleteState : public GameState {
private:
    sf::Text congratsText;
    sf::Sprite nextLevelButton;
    sf::Sprite menuButton;
    
public:
    void enter() override;
    void exit() override;
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    void handleInput(const sf::Event& event) override;
};

// Gestor de estados
class StateManager {
private:
    std::unique_ptr<GameState> currentState;
    std::unique_ptr<GameState> nextState;
    Game* game;
    
public:
    StateManager(Game* gamePtr);
    
    void changeState(std::unique_ptr<GameState> newState);
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void handleInput(const sf::Event& event);
    
    GameState* getCurrentState() const { return currentState.get(); }
};