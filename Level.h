#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <string>

// Constantes de nivel
namespace LevelConstants {
    const int GRID_SIZE = 8;
    const int EMPTY_TILE = 0;
    const int WALL_TILE = 1;
    const int GOAL_TILE = -1;
    const int TRAFFIC_LIGHT_TILE = -2;
}

// Tipos de tiles
enum class TileType {
    EMPTY = 0,
    WALL = 1,
    GOAL = -1,
    TRAFFIC_LIGHT = -2
};

// Información de un tile
struct TileInfo {
    TileType type;
    int height = 0;  // Para tiles con altura (bloques apilados)
    sf::Vector2i position;
};

// Semáforo en el nivel
struct TrafficLight {
    sf::Vector2i gridPosition;
    sf::Vector2f worldPosition;
    sf::Sprite sprite;
    int currentState = 0;  // 0: Rojo, 1: Naranja, 2: Verde
    sf::Clock timer;
    
    // Tiempos para cada estado
    float redTime = 5.0f;
    float orangeTime = 2.0f;
    float greenTime = 5.0f;
    
    void update(float deltaTime);
    void render(sf::RenderWindow& window, const sf::Transform& transform);
};

// Objetivos del nivel
struct LevelObjective {
    std::string description;
    bool completed = false;
    
    enum Type {
        REACH_GOAL,
        COLLECT_ITEM,
        AVOID_OBSTACLES,
        TIME_LIMIT
    } type;
    
    // Datos específicos del objetivo
    sf::Vector2i targetPosition;
    int targetCount = 0;
    float timeLimit = 0.0f;
};

// Clase Level
class Level {
private:
    // Datos del nivel
    int levelId;
    std::string name;
    std::string description;
    
    // Matrices del nivel
    std::array<std::array<int, LevelConstants::GRID_SIZE>, LevelConstants::GRID_SIZE> map3D;
    std::array<std::array<int, LevelConstants::GRID_SIZE>, LevelConstants::GRID_SIZE> map2D;
    
    // Sprites del mundo
    std::array<std::array<sf::Sprite, LevelConstants::GRID_SIZE>, LevelConstants::GRID_SIZE> floorTiles;
    std::array<std::array<sf::Sprite, LevelConstants::GRID_SIZE>, LevelConstants::GRID_SIZE> tiles2D;
    std::vector<sf::Sprite> blockSprites;
    std::vector<sf::Sprite> overlayBlocks;  // Bloques que se dibujan encima del robot
    
    // Posición inicial del robot
    sf::Vector2i robotStartPosition;
    
    // Semáforos
    std::vector<TrafficLight> trafficLights;
    
    // Objetivos
    std::vector<LevelObjective> objectives;
    
    // Transformación isométrica
    sf::Transform isoTransform;
    
    // Referencias a texturas
    const sf::Texture* floorTexture;
    const sf::Texture* wallTexture;
    const sf::Texture* goalTexture;
    const sf::Texture* floor2DTexture;
    const sf::Texture* wall2DTexture;
    const sf::Texture* goal2DTexture;
    const sf::Texture* trafficLightTexture;

public:
    Level(int id, const std::string& levelName);
    ~Level();
    
    // Inicialización
    void initialize();
    void loadFromData(const int map3DData[LevelConstants::GRID_SIZE][LevelConstants::GRID_SIZE],
                     const int map2DData[LevelConstants::GRID_SIZE][LevelConstants::GRID_SIZE]);
    void setupSprites();
    void setupTrafficLights();
    void setupObjectives();
    
    // Actualización
    void update(float deltaTime);
    void updateTrafficLights(float deltaTime);
    void updateObjectives();
    void updateOverlayBlocks(const sf::Vector2i& robotPosition);
    
    // Renderizado
    void render(sf::RenderWindow& window);
    void renderFloor(sf::RenderWindow& window);
    void renderWalls(sf::RenderWindow& window);
    void renderOverlayBlocks(sf::RenderWindow& window);
    void render2D(sf::RenderWindow& window);
    void renderTrafficLights(sf::RenderWindow& window);
    
    // Consultas del mapa
    TileType getTileType(int x, int y) const;
    TileType getTileType(const sf::Vector2i& pos) const;
    int getTileHeight(int x, int y) const;
    bool isValidPosition(int x, int y) const;
    bool isValidPosition(const sf::Vector2i& pos) const;
    bool isWalkable(int x, int y) const;
    bool isWalkable(const sf::Vector2i& pos) const;
    bool isGoal(int x, int y) const;
    bool isGoal(const sf::Vector2i& pos) const;
    
    // Conversiones de coordenadas
    sf::Vector2f gridToWorldPosition(const sf::Vector2i& gridPos) const;
    sf::Vector2i worldToGridPosition(const sf::Vector2f& worldPos) const;
    
    // Gestión de semáforos
    TrafficLight* getTrafficLightAt(const sf::Vector2i& position);
    const std::vector<TrafficLight>& getTrafficLights() const { return trafficLights; }
    
    // Objetivos
    const std::vector<LevelObjective>& getObjectives() const { return objectives; }
    bool areAllObjectivesCompleted() const;
    void checkObjectiveCompletion(const sf::Vector2i& robotPosition);
    
    // Getters
    int getLevelId() const { return levelId; }
    const std::string& getName() const { return name; }
    const std::string& getDescription() const { return description; }
    sf::Vector2i getRobotStartPosition() const { return robotStartPosition; }
    const sf::Transform& getIsoTransform() const { return isoTransform; }
    
    // Setters
    void setRobotStartPosition(const sf::Vector2i& pos) { robotStartPosition = pos; }
    void setDescription(const std::string& desc) { description = desc; }
    
    // Reset
    void reset();
    
private:
    // Métodos auxiliares
    void createFloorSprites();
    void createWallSprites();
    void createBlockSprites();
    void create2DSprites();
    void setupIsoTransform();
    
    // Validaciones
    bool isInBounds(int x, int y) const;
    bool isInBounds(const sf::Vector2i& pos) const;
};

// Manager de niveles
class LevelManager {
private:
    std::vector<std::unique_ptr<Level>> levels;
    int currentLevelIndex = 0;
    
public:
    LevelManager();
    ~LevelManager();
    
    // Carga de niveles
    void loadAllLevels();
    void loadLevel(int levelId);
    
    // Navegación
    Level* getCurrentLevel();
    Level* getLevel(int index);
    bool nextLevel();
    bool previousLevel();
    void setCurrentLevel(int index);
    
    // Información
    int getCurrentLevelIndex() const { return currentLevelIndex; }
    int getTotalLevels() const { return static_cast<int>(levels.size()); }
    bool hasNextLevel() const { return currentLevelIndex < static_cast<int>(levels.size()) - 1; }
    bool hasPreviousLevel() const { return currentLevelIndex > 0; }
    
    // Reset
    void resetCurrentLevel();
    void resetAllLevels();
};