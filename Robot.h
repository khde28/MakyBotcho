#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

// Constantes para el robot
namespace RobotConstants {
    const int FRAME_WIDTH = 40;
    const int FRAME_HEIGHT = 30;
    const float ANIMATION_SPEED = 0.3f;
    const float ROTATION_TIME = 1.5f;
    const float MOVEMENT_SPEED = 1.0f;
}

// Direcciones del robot
enum class Direction {
    SOUTH_EAST = 0,  // Inicial
    SOUTH_WEST = 1,
    NORTH_WEST = 2,
    NORTH_EAST = 3
};

// Estados del robot
struct RobotState {
    bool miraNE = false;
    bool miraNO = false;
    bool miraSO = false;
    bool miraSE = true;  // Estado inicial
};

// Tipos de movimiento
enum class MovementType {
    NONE = 0,
    FORWARD = 1,
    TURN_RIGHT = 2,
    TURN_LEFT = 3,
    LIGHT = 4,
    FUNCTION_F1 = 5,
    LOOP = 6,
    CURRENT = 7
};

class Robot {
private:
    sf::Sprite sprite;
    sf::Sprite sprite2D;
    
    // Animación
    std::vector<sf::IntRect> frontFrames;
    std::vector<sf::IntRect> backFrames;
    sf::IntRect lightFrontFrame;
    sf::IntRect lightBackFrame;
    
    int currentFrame;
    sf::Clock animationClock;
    sf::Clock rotationClock;
    
    // Movimiento
    sf::Vector2f targetPosition;
    sf::Vector2f currentPosition;
    bool isMoving;
    bool isRotating;
    bool isColliding;
    
    // Estado y dirección
    Direction currentDirection;
    RobotState state;
    
    // Grid
    sf::Vector2i gridPosition;
    
    // Constantes del mundo
    float tileSize;
    float xIso, yIso;
    sf::Vector2f worldOrigin;

public:
    Robot(float tileSize, float xIso, float yIso, const sf::Vector2f& worldOrigin);
    
    // Inicialización
    void initialize();
    void setupAnimationFrames();
    
    // Actualización
    void update(float deltaTime);
    void updateAnimation(float deltaTime);
    void updateMovement(float deltaTime);
    void updateRotation(float deltaTime);
    
    // Renderizado
    void render(sf::RenderWindow& window, const sf::Transform& isoTransform);
    void render2D(sf::RenderWindow& window);
    
    // Movimientos
    void moveForward();
    void turnRight();
    void turnLeft();
    void turnOnLight();
    
    // Control de estado
    void setMoving(bool moving) { isMoving = moving; }
    void setRotating(bool rotating) { isRotating = rotating; }
    void setColliding(bool colliding) { isColliding = colliding; }
    
    // Getters
    bool getIsMoving() const { return isMoving; }
    bool getIsRotating() const { return isRotating; }
    bool getIsColliding() const { return isColliding; }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
    sf::Vector2f getTargetPosition() const { return targetPosition; }
    sf::Vector2i getGridPosition() const { return gridPosition; }
    Direction getCurrentDirection() const { return currentDirection; }
    const RobotState& getState() const { return state; }
    
    // Setters
    void setPosition(const sf::Vector2f& position);
    void setTargetPosition(const sf::Vector2f& target) { targetPosition = target; }
    void setGridPosition(const sf::Vector2i& gridPos);
    
    // Reset
    void reset();
    void resetToInitialPosition();
    
private:
    // Métodos privados de ayuda
    void updateDirection();
    void updateSpriteScale();
    void updateTextureRect();
    sf::Vector2f calculateMovementVector() const;
    sf::Vector2f gridToWorldPosition(const sf::Vector2i& gridPos) const;
    sf::Vector2i worldToGridPosition(const sf::Vector2f& worldPos) const;
};