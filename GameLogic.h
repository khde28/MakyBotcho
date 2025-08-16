#pragma once
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <queue>

// Forward declarations
class Robot;
class Level;
class TrafficLight;

// Constantes del juego
namespace GameConstants {
    const int GRID_SIZE = 8;
    const float TILE_SIZE = 50.0f;
    const float X_ISO = 70.71067f;  // sqrt(2 * TILE_SIZE * TILE_SIZE)
    const float Y_ISO = 35.35534f;  // X_ISO / 2.0f
    const sf::Vector2f WORLD_ORIGIN(300.0f, 150.0f);
    
    const int MAX_MAIN_INSTRUCTIONS = 12;
    const int MAX_F1_INSTRUCTIONS = 8;
    const int MAX_LOOP_INSTRUCTIONS = 4;
    const int MAX_IF_INSTRUCTIONS = 8;
    const int MAX_ELSE_INSTRUCTIONS = 8;
}

// Estados de ejecución
enum class ExecutionState {
    STOPPED,
    RUNNING,
    PAUSED,
    FINISHED,
    ERROR
};

// Contexto de ejecución de instrucciones
struct ExecutionContext {
    int mainCounter = 0;
    int f1Counter = 0;
    int loopCounter = 0;
    int ifCounter = 0;
    int elseCounter = 0;
    
    int currentLoopIteration = 0;
    int maxLoopIterations = 0;
    
    bool isInFunction = false;
    bool isInLoop = false;
    bool isInConditional = false;
    bool conditionalResult = true;
    
    int trafficLightState = -1;  // Para condicionales basadas en semáforo
    
    void reset() {
        mainCounter = 0;
        f1Counter = 0;
        loopCounter = 0;
        ifCounter = 0;
        elseCounter = 0;
        currentLoopIteration = 0;
        maxLoopIterations = 0;
        isInFunction = false;
        isInLoop = false;
        isInConditional = false;
        conditionalResult = true;
        trafficLightState = -1;
    }
};

// Clase principal de lógica del juego
class GameLogic {
private:
    // Referencias a componentes
    Robot* robot;
    Level* currentLevel;
    
    // Estado de ejecución
    ExecutionState state;
    ExecutionContext context;
    
    // Arrays de instrucciones
    std::array<int, GameConstants::MAX_MAIN_INSTRUCTIONS> mainInstructions;
    std::array<int, GameConstants::MAX_F1_INSTRUCTIONS> f1Instructions;
    std::array<int, GameConstants::MAX_LOOP_INSTRUCTIONS> loopInstructions;
    std::array<int, GameConstants::MAX_IF_INSTRUCTIONS> ifInstructions;
    std::array<int, GameConstants::MAX_ELSE_INSTRUCTIONS> elseInstructions;
    
    // Control de tiempo
    sf::Clock executionClock;
    float instructionDelay = 0.5f;  // Delay entre instrucciones
    
    // Sistema de semáforos
    std::vector<TrafficLight*> trafficLights;
    
public:
    GameLogic();
    ~GameLogic();
    
    // Inicialización
    void initialize(Robot* robotPtr, Level* levelPtr);
    void setInstructions(const std::array<int, GameConstants::MAX_MAIN_INSTRUCTIONS>& main,
                        const std::array<int, GameConstants::MAX_F1_INSTRUCTIONS>& f1,
                        const std::array<int, GameConstants::MAX_LOOP_INSTRUCTIONS>& loop,
                        const std::array<int, GameConstants::MAX_IF_INSTRUCTIONS>& ifInstr,
                        const std::array<int, GameConstants::MAX_ELSE_INSTRUCTIONS>& elseInstr);
    
    // Control de ejecución
    void startExecution();
    void pauseExecution();
    void resumeExecution();
    void stopExecution();
    void resetExecution();
    
    // Actualización principal
    void update(float deltaTime);
    
    // Ejecución de instrucciones
    bool executeNextInstruction();
    bool executeMainInstruction();
    bool executeF1Instruction();
    bool executeLoopInstruction();
    bool executeIfInstruction();
    bool executeElseInstruction();
    
    // Instrucciones específicas
    void executeMoveForward();
    void executeTurnRight();
    void executeTurnLeft();
    void executeTurnOnLight();
    void executeFunction();
    void executeLoop();
    void executeConditional();
    
    // Validación y colisiones
    bool canMoveToPosition(const sf::Vector2i& gridPos);
    bool checkCollision(const sf::Vector2i& gridPos);
    bool checkGoalReached();
    bool checkTrafficLightCondition();
    
    // Getters
    ExecutionState getState() const { return state; }
    const ExecutionContext& getContext() const { return context; }
    bool isRunning() const { return state == ExecutionState::RUNNING; }
    bool isPaused() const { return state == ExecutionState::PAUSED; }
    bool isFinished() const { return state == ExecutionState::FINISHED; }
    
    // Setters
    void setInstructionDelay(float delay) { instructionDelay = delay; }
    void setLoopCount(int count) { context.maxLoopIterations = count; }
    
private:
    // Métodos auxiliares
    int getCurrentInstruction();
    void advanceCounter();
    bool shouldExecuteConditional();
    void handleCollision();
    void handleGoalReached();
    void handleExecutionError();
    
    // Manejo de arrays de instrucciones
    template<size_t N>
    bool hasMoreInstructions(const std::array<int, N>& instructions, int counter);
    
    template<size_t N>
    int getInstructionAt(const std::array<int, N>& instructions, int index);
};