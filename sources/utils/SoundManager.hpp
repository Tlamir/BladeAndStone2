#pragma once
#include <raylib.h>
#include <unordered_map>
#include <string>

class SoundManager
{
private:
    static SoundManager* instance;
    std::unordered_map<std::string, Sound> soundEffects;

    // Private constructor for singleton
    SoundManager();

    // Prevent copying
    SoundManager(const SoundManager&) = delete;
    SoundManager& operator=(const SoundManager&) = delete;

public:
    static SoundManager* getInstance();
    ~SoundManager();

    // Core functions
    void initialize();
    void cleanup();

    // Sound effects functions
    void loadSoundEffect(const std::string& name, const std::string& filepath);
    void playSoundEffect(const std::string& name);
    void stopSoundEffect(const std::string& name);
    void setSoundEffectVolume(const std::string& name, float volume);
    void stopAllSounds();
};