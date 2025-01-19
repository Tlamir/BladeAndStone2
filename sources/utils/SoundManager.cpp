#include "SoundManager.hpp"
#include <Constants.hpp>

SoundManager* SoundManager::instance = nullptr;

SoundManager* SoundManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SoundManager();
    }
    return instance;
}

SoundManager::SoundManager()
{
    InitAudioDevice();
}

SoundManager::~SoundManager()
{
    cleanup();
    CloseAudioDevice();
}

void SoundManager::initialize()
{
    // Load all sound effects
    loadSoundEffect("sword", AppConstants::GetAssetPath("Audio/sword.wav"));
    loadSoundEffect("spell", AppConstants::GetAssetPath("Audio/spell.ogg"));
    loadSoundEffect("gameStart", AppConstants::GetAssetPath("Audio/gameStart.wav"));
    loadSoundEffect("gameOver", AppConstants::GetAssetPath("Audio/gameOver.wav"));
}

void SoundManager::cleanup()
{
    // Unload all sound effects
    for (auto& [name, sound] : soundEffects)
    {
        UnloadSound(sound);
    }
    soundEffects.clear();
}

void SoundManager::loadSoundEffect(const std::string& name, const std::string& filepath)
{
    Sound sound = LoadSound(filepath.c_str());
    soundEffects[name] = sound;
}

void SoundManager::playSoundEffect(const std::string& name)
{
    if (soundEffects.find(name) != soundEffects.end())
    {
        PlaySound(soundEffects[name]);
    }
}

void SoundManager::stopSoundEffect(const std::string& name)
{
    if (soundEffects.find(name) != soundEffects.end())
    {
        StopSound(soundEffects[name]);
    }
}

void SoundManager::setSoundEffectVolume(const std::string& name, float volume)
{
    if (soundEffects.find(name) != soundEffects.end())
    {
        SetSoundVolume(soundEffects[name], volume);
    }
}

void SoundManager::stopAllSounds()
{
    for (auto& [name, sound] : soundEffects)
    {
        StopSound(sound);
    }
}