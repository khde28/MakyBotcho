#pragma once
#include <string>
#include <array>

// Versión del juego
namespace Version {
    const int MAJOR = 1;
    const int MINOR = 0;
    const int PATCH = 0;
    const std::string VERSION_STRING = "1.0.0";
}

// Configuración de ventana
namespace Window {
    const int DEFAULT_WIDTH = 1000;
    const int DEFAULT_HEIGHT = 750;
    const int MIN_WIDTH = 800;
    const int MIN_HEIGHT = 600;
    const std::string TITLE = "Makibot v" + Version::VERSION_STRING;
}

// Configuración de renderizado
namespace Rendering {
    const int TARGET_FPS = 60;
    const bool VSYNC_ENABLED = true;
    const int ANTIALIASING_LEVEL = 4;
}

// Configuración del mundo del juego
namespace World {
    const int GRID_SIZE = 8;
    const float TILE_SIZE = 50.0f;
    const float ISO_X = 70.71067f;  // sqrt(2 * TILE_SIZE * TILE_SIZE)
    const float ISO_Y = 35.35534f;  // ISO_X / 2.0f
    const float ISO_ORIGIN_X = 300.0f;
    const float ISO_ORIGIN_Y = 150.0f;
}

// Configuración de la UI
namespace UI {
    const float BUTTON_HOVER_SCALE = 1.06f;
    const float BUTTON_CLICK_SCALE = 0.95f;
    const float ANIMATION_SPEED = 0.2f;
    const int INSTRUCTION_PANEL_WIDTH = 200;
    const int INSTRUCTION_PANEL_HEIGHT = 150;
}

// Configuración de animaciones
namespace Animation {
    const float ROBOT_MOVE_SPEED = 100.0f;  // pixels por segundo
    const float ROBOT_ROTATION_TIME = 0.5f;  // segundos
    const float INSTRUCTION_DELAY = 0.8f;    // segundos entre instrucciones
}

// Configuración de audio
namespace Audio {
    const float MASTER_VOLUME = 100.0f;
    const float MUSIC_VOLUME = 30.0f;
    const float SFX_VOLUME = 70.0f;
    const int MAX_SOUND_CHANNELS = 16;
}

// Rutas de archivos
namespace Paths {
    const std::string IMAGES = "images/";
    const std::string SOUNDS = "sounds/";
    const std::string FONTS = "fonts/";
    const std::string CONFIG = "config/";
    const std::string SAVES = "saves/";
    
    // Archivos específicos
    const std::string SETTINGS_FILE = CONFIG + "settings.ini";
    const std::string SAVE_FILE = SAVES + "progress.save";
}

// Configuración de input
namespace Input {
    const float MOUSE_DOUBLE_CLICK_TIME = 0.5f;  // segundos
    const float MOUSE_DRAG_THRESHOLD = 5.0f;     // pixels
}

// Configuración de niveles
namespace Levels {
    const int MAX_LEVELS = 10;
    const int MAX_INSTRUCTIONS_MAIN = 12;
    const int MAX_INSTRUCTIONS_F1 = 8;
    const int MAX_INSTRUCTIONS_LOOP = 4;
    const int MAX_INSTRUCTIONS_IF = 8;
    const int MAX_INSTRUCTIONS_ELSE = 8;
    const int MAX_LOOP_ITERATIONS = 5;
}

// Tipos de tiles en el mapa
namespace TileTypes {
    const int EMPTY = 0;
    const int WALL = 1;
    const int GOAL = -1;
    const int TRAFFIC_LIGHT = -2;
}

// Tipos de instrucciones
namespace Instructions {
    const int NONE = 0;
    const int MOVE_FORWARD = 1;
    const int TURN_RIGHT = 2;
    const int TURN_LEFT = 3;
    const int LIGHT = 4;
    const int FUNCTION_F1 = 5;
    const int LOOP = 6;
    const int CURRENT = 7;  // Marcador para instrucción actual
}

// Estados del semáforo
namespace TrafficLight {
    const int RED = 0;
    const int ORANGE = 1;
    const int GREEN = 2;
    
    const float RED_TIME = 5.0f;
    const float ORANGE_TIME = 2.0f;
    const float GREEN_TIME = 5.0f;
}

// Colores del juego
namespace Colors {
    const unsigned int BACKGROUND = 0xA9A9A9FF;  // Gris oscuro
    const unsigned int UI_PANEL = 0x2C3E50FF;    // Azul oscuro
    const unsigned int UI_BUTTON = 0x3498DBFF;   // Azul
    const unsigned int UI_BUTTON_HOVER = 0x5DADE2FF;  // Azul claro
    const unsigned int UI_TEXT = 0xFFFFFFFF;     // Blanco
    const unsigned int UI_TEXT_SECONDARY = 0xBDC3C7FF;  // Gris claro
    const unsigned int SUCCESS = 0x2ECC71FF;     // Verde
    const unsigned int ERROR = 0xE74C3CFF;       // Rojo
    const unsigned int WARNING = 0xF39C12FF;     // Naranja
}

// Configuración de debug
namespace Debug {
    const bool SHOW_FPS = true;
    const bool SHOW_GRID = false;
    const bool SHOW_COLLISION_BOXES = false;
    const bool ENABLE_CONSOLE = true;
    const bool LOG_INSTRUCTIONS = true;
}

// Macros útiles
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))
#define STRINGIFY(x) #x
#define TO_STRING(x) STRINGIFY(x)

// Funciones inline de utilidad
namespace Utils {
    inline float degreesToRadians(float degrees) {
        return degrees * 3.14159265f / 180.0f;
    }
    
    inline float radiansToDegrees(float radians) {
        return radians * 180.0f / 3.14159265f;
    }
    
    inline float lerp(float a, float b, float t) {
        return a + t * (b - a);
    }
    
    inline float clamp(float value, float min, float max) {
        return value < min ? min : (value > max ? max : value);
    }
}