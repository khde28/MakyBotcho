#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <functional>
#include <memory>

// Constantes de UI
namespace UIConstants {
    const float BUTTON_SCALE = 1.06f;
    const float HOVER_SCALE = 0.9f;
    const float NORMAL_SCALE = 0.8f;
    const int INSTRUCTION_ARRAY_SIZE = 12;
    const int F1_ARRAY_SIZE = 8;
    const int LOOP_ARRAY_SIZE = 4;
    const int IF_ARRAY_SIZE = 8;
    const int ELSE_ARRAY_SIZE = 8;
}

// Clase base para elementos de UI
class UIElement {
protected:
    sf::Vector2f position;
    bool visible = true;
    bool enabled = true;

public:
    virtual ~UIElement() = default;
    
    virtual void update(float deltaTime) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual bool handleClick(const sf::Vector2f& mousePos) = 0;
    virtual void handleHover(const sf::Vector2f& mousePos) = 0;
    
    // Getters y setters
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    sf::Vector2f getPosition() const { return position; }
    void setVisible(bool vis) { visible = vis; }
    bool isVisible() const { return visible; }
    void setEnabled(bool en) { enabled = en; }
    bool isEnabled() const { return enabled; }
};

// Botón simple
class Button : public UIElement {
private:
    sf::Sprite sprite;
    std::function<void()> onClick;
    sf::Sound* clickSound;
    bool isHovered = false;
    float hoverScale = UIConstants::BUTTON_SCALE;
    float normalScale = 1.0f;

public:
    Button(const sf::Texture& texture, const sf::Vector2f& position);
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool handleClick(const sf::Vector2f& mousePos) override;
    void handleHover(const sf::Vector2f& mousePos) override;
    
    void setOnClick(std::function<void()> callback) { onClick = callback; }
    void setClickSound(sf::Sound* sound) { clickSound = sound; }
    void setTexture(const sf::Texture& texture) { sprite.setTexture(texture); }
    void setScale(float scale) { sprite.setScale(scale, scale); }
    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }
};

// Botón de comando (para instrucciones)
class CommandButton : public Button {
private:
    int commandId;

public:
    CommandButton(const sf::Texture& texture, const sf::Vector2f& position, int id);
    
    int getCommandId() const { return commandId; }
};

// Panel de instrucciones
class InstructionPanel : public UIElement {
private:
    std::array<int, UIConstants::INSTRUCTION_ARRAY_SIZE> instructions;
    std::vector<sf::Sprite> instructionSprites;
    const std::vector<sf::Texture*> textures;
    
    Button deleteButton;
    Button toggleButton;
    
    bool isActive = false;
    sf::Vector2f panelOffset;
    
public:
    template<size_t N>
    InstructionPanel(const sf::Vector2f& position, 
                    const std::vector<sf::Texture*>& textures,
                    std::array<int, N>& instructionArray);
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool handleClick(const sf::Vector2f& mousePos) override;
    void handleHover(const sf::Vector2f& mousePos) override;
    
    void addInstruction(int instruction);
    void removeLastInstruction();
    void clear();
    void setActive(bool active) { isActive = active; }
    bool getIsActive() const { return isActive; }
    
private:
    void updateSprites();
};

// Counter (para bucles)
class Counter : public UIElement {
private:
    int value = 0;
    int minValue = 0;
    int maxValue = 5;
    
    sf::Sprite numberSprite;
    Button incrementButton;
    Button decrementButton;
    
    std::vector<sf::Texture*> numberTextures;

public:
    Counter(const sf::Vector2f& position, 
           const std::vector<sf::Texture*>& numberTextures,
           const sf::Texture& incrementTexture,
           const sf::Texture& decrementTexture);
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool handleClick(const sf::Vector2f& mousePos) override;
    void handleHover(const sf::Vector2f& mousePos) override;
    
    int getValue() const { return value; }
    void setValue(int val);
    void increment();
    void decrement();
    void setRange(int min, int max);
};

// Toggle Button (para IF/ELSE)
class ToggleButton : public UIElement {
private:
    sf::Sprite sprite;
    const sf::Texture& texture1;
    const sf::Texture& texture2;
    bool state = true;  // true = texture1, false = texture2
    std::function<void(bool)> onToggle;
    sf::Sound* clickSound;

public:
    ToggleButton(const sf::Vector2f& position,
                const sf::Texture& tex1,
                const sf::Texture& tex2);
    
    void update(float deltaTime) override;
    void render(sf::RenderWindow& window) override;
    bool handleClick(const sf::Vector2f& mousePos) override;
    void handleHover(const sf::Vector2f& mousePos) override;
    
    void setState(bool newState);
    bool getState() const { return state; }
    void setOnToggle(std::function<void(bool)> callback) { onToggle = callback; }
    void setClickSound(sf::Sound* sound) { clickSound = sound; }
};

// UI Manager principal
class UIManager {
private:
    std::vector<std::unique_ptr<UIElement>> elements;
    std::vector<std::unique_ptr<CommandButton>> commandButtons;
    
    // Paneles específicos
    std::unique_ptr<InstructionPanel> mainPanel;
    std::unique_ptr<InstructionPanel> f1Panel;
    std::unique_ptr<InstructionPanel> loopPanel;
    std::unique_ptr<InstructionPanel> ifPanel;
    std::unique_ptr<InstructionPanel> elsePanel;
    
    // Controles
    std::unique_ptr<Counter> loopCounter;
    std::unique_ptr<ToggleButton> ifElseToggle;
    std::unique_ptr<Button> startButton;
    std::unique_ptr<Button> resetButton;
    std::unique_ptr<Button> resetAllButton;
    std::unique_ptr<Button> pauseButton;
    
    // Audio
    sf::Sound clickSound1;
    sf::Sound clickSound2;

public:
    UIManager();
    ~UIManager();
    
    void initialize();
    void update(float deltaTime);
    void render(sf::RenderWindow& window);
    void handleInput(const sf::Event& event);
    
    // Getters para los datos de las instrucciones
    const std::array<int, UIConstants::INSTRUCTION_ARRAY_SIZE>& getMainInstructions() const;
    const std::array<int, UIConstants::F1_ARRAY_SIZE>& getF1Instructions() const;
    const std::array<int, UIConstants::LOOP_ARRAY_SIZE>& getLoopInstructions() const;
    const std::array<int, UIConstants::IF_ARRAY_SIZE>& getIfInstructions() const;
    const std::array<int, UIConstants::ELSE_ARRAY_SIZE>& getElseInstructions() const;
    
    int getLoopCount() const;
    bool getIfElseState() const;
    
    // Control de paneles activos
    void setActivePanel(const std::string& panelName);
    std::string getActivePanel() const;
    
    // Callbacks
    void setOnStartGame(std::function<void()> callback);
    void setOnResetGame(std::function<void()> callback);
    void setOnResetAll(std::function<void()> callback);
    void setOnPauseGame(std::function<void()> callback);

private:
    void setupCommandButtons();
    void setupPanels();
    void setupControls();
    void setupAudio();
};