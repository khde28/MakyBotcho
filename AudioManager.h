#pragma once
#include <SFML/Audio.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>

// Configuración de audio
namespace AudioConfig {
    const float DEFAULT_VOLUME = 100.0f;
    const float DEFAULT_MUSIC_VOLUME = 30.0f;
    const float DEFAULT_SFX_VOLUME = 70.0f;
    const int MAX_SOUND_CHANNELS = 16;
}

// Tipos de audio
enum class AudioType {
    SFX,
    MUSIC,
    VOICE
};

// Canal de audio para efectos de sonido
struct SoundChannel {
    sf::Sound sound;
    bool inUse = false;
    float priority = 0.0f;
    
    void play(const sf::SoundBuffer& buffer, float volume = AudioConfig::DEFAULT_SFX_VOLUME, float pitch = 1.0f);
    void stop();
    bool isPlaying() const;
};

// Información de música
struct MusicInfo {
    std::string filename;
    bool loop = true;
    float volume = AudioConfig::DEFAULT_MUSIC_VOLUME;
    float fadeInTime = 0.0f;
    float fadeOutTime = 0.0f;
};

// Manager de audio
class AudioManager {
private:
    // Canales de sonido
    std::vector<SoundChannel> soundChannels;
    
    // Música
    sf::Music currentMusic;
    std::string currentMusicName;
    bool musicEnabled = true;
    bool sfxEnabled = true;
    
    // Volúmenes globales
    float masterVolume = AudioConfig::DEFAULT_VOLUME;
    float musicVolume = AudioConfig::DEFAULT_MUSIC_VOLUME;
    float sfxVolume = AudioConfig::DEFAULT_SFX_VOLUME;
    
    // Fade para música
    bool isFading = false;
    float fadeTimer = 0.0f;
    float fadeDuration = 0.0f;
    float fadeStartVolume = 0.0f;
    float fadeTargetVolume = 0.0f;
    std::string nextMusicName;
    
    // Playlist de música de fondo
    std::vector<std::string> backgroundMusicPlaylist;
    int currentPlaylistIndex = 0;
    bool shuffleMode = false;
    bool repeatMode = true;

public:
    AudioManager();
    ~AudioManager();
    
    // Inicialización
    bool initialize();
    void cleanup();
    
    // Actualización
    void update(float deltaTime);
    
    // Control de efectos de sonido
    void playSFX(const std::string& soundName, float volume = -1.0f, float pitch = 1.0f, float priority = 1.0f);
    void stopSFX(const std::string& soundName);
    void stopAllSFX();
    
    // Control de música
    bool playMusic(const std::string& musicName, bool loop = true, float fadeInTime = 0.0f);
    void stopMusic(float fadeOutTime = 0.0f);
    void pauseMusic();
    void resumeMusic();
    void setMusicVolume(float volume);
    void fadeMusic(float targetVolume, float duration);
    
    // Playlist
    void setBackgroundPlaylist(const std::vector<std::string>& playlist);
    void playNextInPlaylist();
    void playPreviousInPlaylist();
    void setShuffleMode(bool enabled) { shuffleMode = enabled; }
    void setRepeatMode(bool enabled) { repeatMode = enabled; }
    
    // Control de volumen
    void setMasterVolume(float volume);
    void setSFXVolume(float volume);
    void setMusicGlobalVolume(float volume);
    
    float getMasterVolume() const { return masterVolume; }
    float getSFXVolume() const { return sfxVolume; }
    float getMusicVolume() const { return musicVolume; }
    
    // Habilitación/deshabilitación
    void enableMusic(bool enabled);
    void enableSFX(bool enabled);
    bool isMusicEnabled() const { return musicEnabled; }
    bool isSFXEnabled() const { return sfxEnabled; }
    
    // Estado
    bool isMusicPlaying() const;
    const std::string& getCurrentMusicName() const { return currentMusicName; }
    
    // Sonidos preconfigurados del juego
    void playClickSound();
    void playErrorSound();
    void playSuccessSound();
    void playMovementSound();
    void playRotationSound();
    void playGoalSound();
    void playCollisionSound();
    
private:
    // Gestión de canales
    SoundChannel* findAvailableChannel();
    SoundChannel* findChannelWithLowestPriority();
    
    // Gestión de fade
    void updateMusicFade(float deltaTime);
    void startFade(float targetVolume, float duration, const std::string& nextMusic = "");
    
    // Aplicación de volúmenes
    float calculateFinalVolume(float baseVolume, AudioType type) const;
    void updateAllVolumes();
    
    // Playlist
    void playPlaylistMusic(int index);
    int getRandomPlaylistIndex() const;
};

// Clase auxiliar para efectos de sonido con configuración
class SoundEffect {
private:
    std::string soundName;
    float baseVolume;
    float basePitch;
    float priority;
    bool isLooped;
    
public:
    SoundEffect(const std::string& name, float volume = 1.0f, float pitch = 1.0f, float prio = 1.0f);
    
    void play(float volumeMultiplier = 1.0f, float pitchMultiplier = 1.0f);
    void stop();
    
    // Configuración
    void setVolume(float volume) { baseVolume = volume; }
    void setPitch(float pitch) { basePitch = pitch; }
    void setPriority(float prio) { priority = prio; }
    void setLooped(bool loop) { isLooped = loop; }
    
    // Getters
    float getVolume() const { return baseVolume; }
    float getPitch() const { return basePitch; }
    float getPriority() const { return priority; }
    bool getIsLooped() const { return isLooped; }
};

// Sonidos predefinidos del juego
namespace GameSounds {
    extern SoundEffect ButtonClick;
    extern SoundEffect ButtonHover;
    extern SoundEffect InstructionAdd;
    extern SoundEffect InstructionDelete;
    extern SoundEffect RobotMove;
    extern SoundEffect RobotRotate;
    extern SoundEffect RobotCollision;
    extern SoundEffect LevelComplete;
    extern SoundEffect GameStart;
    extern SoundEffect Error;
    
    void initializeGameSounds();
}