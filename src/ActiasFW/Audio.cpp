/*
File:		Audio.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.16@11:28
Purpose:	Define variables and implement functions found in Audio.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement AudioManager class functions

	bool AudioManager::initialize(float effectVolume, float musicVolume) {
		ACTIASFW_LOG(Audio, Info, "Initializing SoLoud library and audio ",
			"system");
		if (m_initialized) {
			ACTIASFW_LOG(Audio, Error, "The audio manager has already been ",
				"initialized");
			return false;
		}
		if (m_soloud.init() != SoLoud::SO_NO_ERROR) {
			ACTIASFW_LOG(Audio, Error, "Failed to initialize SoLoud library");
			return false;
		}
		setEffectVolume(effectVolume);
		setMusicVolume(musicVolume);
		m_initialized = true;
		return true;
	}

	bool AudioManager::playEffect(const std::string& fileName) {
		if (m_audio.find(fileName) == m_audio.end()) {
			if (!loadEffect(fileName)) {
				ACTIASFW_LOG(Audio, Warning, "Failed to load sound effect \"",
					fileName, "\"");
				return false;
			}
		}
		m_soloud.play(m_audio.at(fileName), m_effectVolume);
		return true;
	}

	bool AudioManager::isMusicPaused() const {
		return m_musicPaused;
	}

	bool AudioManager::playMusic(const std::string& fileName) {
		if (fileName.empty()) {
			if (m_currentMusicTrack == 0) {
				return false;
			}
			if (!m_soloud.getPause(m_currentMusicTrack)) {
				return false;
			}
			ACTIASFW_LOG(Audio, Info, "Resuming current music track");
			m_soloud.setPause(m_currentMusicTrack, false);
		}
		else {
			if (m_audio.find(fileName) == m_audio.end()) {
				if (!loadEffect(fileName)) {
					ACTIASFW_LOG(Audio, Info, "Failed to load music track \"",
						fileName, "\"");
					return false;
				}
			}
			stopMusic();
			ACTIASFW_LOG(Audio, Info, "Playing music track \"", fileName, "\"");
			m_currentMusicTrack = m_soloud.playBackground(m_audio.at(fileName),
				m_musicVolume);
			m_soloud.setLooping(m_currentMusicTrack, true);
		}
		m_musicPaused = false;
		return true;
	}

	bool AudioManager::pauseMusic() {
		if (m_currentMusicTrack == 0 || m_musicPaused) {
			return false;
		}
		ACTIASFW_LOG(Audio, Info, "Pausing current music track");
		m_soloud.setPause(m_currentMusicTrack, true);
		m_musicPaused;
		return true;
	}

	bool AudioManager::stopMusic() {
		if (m_currentMusicTrack == 0) {
			return false;
		}
		ACTIASFW_LOG(Audio, Info, "Stopping current music track");
		m_soloud.stop(m_currentMusicTrack);
		m_currentMusicTrack = 0;
		m_musicPaused = false;
		return true;
	}

	bool AudioManager::destroy() {
		ACTIASFW_LOG(Audio, Info, "Destroying audio manager");
		if (!m_initialized) {
			ACTIASFW_LOG(Audio, Info, "The audio manager has already been ",
				"destroyed");
			return false;
		}
		m_soloud.stopAll();
		m_audio.clear();
		m_currentMusicTrack = 0;
		m_musicPaused = false;
		m_initialized = false;
		return true;
	}

	float AudioManager::getEffectVolume() const {
		return m_effectVolume;
	}

	void AudioManager::setEffectVolume(float effectVolume) {
		if (effectVolume < 0.0f) {
			m_effectVolume = 0.0f;
		}
		else if (effectVolume > 1.0f) {
			m_effectVolume = 1.0f;
		}
		else {
			m_effectVolume = effectVolume;
		}
	}

	float AudioManager::getMusicVolume() {
		return m_musicVolume;
	}

	void AudioManager::setMusicVolume(float musicVolume) {
		if (musicVolume < 0.0f) {
			m_musicVolume = 0.0f;
		}
		else if (musicVolume > 1.0f) {
			m_musicVolume = 1.0f;
		}
		else {
			m_musicVolume = musicVolume;
		}
		m_soloud.setVolume(m_currentMusicTrack, m_musicVolume);
	}

	bool AudioManager::loadEffect(const std::string& fileName) {
		ACTIASFW_LOG(Audio, Info, "Loading sound effect \"", fileName, "\"");
		m_audio[fileName] = SoLoud::Wav();
		if (m_audio[fileName].load(fileName.c_str())
			!= SoLoud::SO_NO_ERROR) {
			m_audio.erase(m_audio.find(fileName));
			return false;
		}
		return true;
	}
}