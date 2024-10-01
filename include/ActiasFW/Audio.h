/*
File:		Audio.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.16@11:28
Purpose:	Contains audio loading and playing utilities for ActiasFW
			applications
*/

#ifndef ACTIASFW_AUDIO_H
#define ACTIASFW_AUDIO_H

#include <SoLoud/soloud.h>
#include <SoLoud/soloud_wav.h>

#include <string>
#include <map>

namespace ActiasFW {
	// The sound effect and music system for ActiasFW applications
	class AudioManager {
	public:
		/*
		Initialize the SoLoud library and this audio manager's memory
		Parameter: float - The initial sound effect volume between 0.0 and 1.0
		Parameter: float - The initial music volume between 0.0 and 1.0
		Returns: bool - Whether the SoLoud library could be initialized
		*/
		bool initialize(float, float);
		/*
		Play a sound effect by its file name
		Parameter: const std::string& fileName - The file name of the WAV effect
		Returns: bool - Whether the sound effect could be loaded and played
		*/
		bool playEffect(const std::string&);
		/*
		Test if the currently playing music track is paused if one is present
		Returns: bool - Whether the music track is present and is paused
		*/
		bool isMusicPaused() const;
		/*
		Play a music track or resume the current music track
		Parameter: const std::string& fileName - The file name of the music
		track to play or empty string to resume the current track
		Returns: bool - Whether the music track could be played or resumed
		*/
		bool playMusic(const std::string& = "");
		/*
		Pause the current music track if one is playing
		Returns: bool - Whether the music track was present and could be paused
		*/
		bool pauseMusic();
		/*
		Stop the current music track if one is playing or paused
		Returns: bool - Whether the music track was present and could be stopped
		*/
		bool stopMusic();
		/*
		Stop all sounds playing and free this audio manager's memory
		Returns: bool - Whether the audio manager could be destroyed
		*/
		bool destroy();
		/*
		Get the current sound effect volume of the audio in the ActiasFW
		application
		Returns: float - The current sound effect volume between 0.0 and 1.0
		*/
		float getEffectVolume() const;
		/*
		Set the sound effect volume of the audio in the ActiasFW application
		Parameter: float effectVolume - The new sound effect volume between 0.0
		and 1.0
		*/
		void setEffectVolume(float);
		/*
		Get the current music volume of the audio in the ActiasFW application
		Returns: float - The current music volume betwen 0.0 and 1.0
		*/
		float getMusicVolume();
		/*
		Set the music volume of the audio in the ActiasFW application
		Parameter: float musicVolume - The new music volume between 0.0 and 1.0
		*/
		void setMusicVolume(float);

	private:
		// Whether this audio manager has been initialized
		bool m_initialized = false;
		// The instance of the SoLoud library used by this audio manager
		SoLoud::Soloud m_soloud;
		// The set of audio files loaded into memory associated with their file
		// names
		std::map<std::string, SoLoud::Wav> m_audio;
		// The SoLoud library's handle for the current music track
		SoLoud::handle m_currentMusicTrack = 0;
		// Whether a music track present and is paused
		bool m_musicPaused = false;
		// The current sound effect volume
		float m_effectVolume = 0.0f;
		// The current music volume
		float m_musicVolume = 0.0f;

		/*
		Load a WAV file into memory by its file name using the SoLoud library
		Parameter: const std::string& fileName - The name of the WAV file to
		load
		Returns: bool - Whether the WAV file could be loaded
		*/
		bool loadEffect(const std::string&);
	};
}

#endif