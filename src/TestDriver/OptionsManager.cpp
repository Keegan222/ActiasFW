/*
File:		OptionsManager.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.19@15:46
Purpose:	Implement functions found in the OptionsManager class of
			TestDriver.h
*/

#include "TestDriver.h"

glm::ivec2 OptionsManager::WindowDimensions = glm::ivec2();
bool OptionsManager::Fullscreen = false;
GLFWmonitor* OptionsManager::Monitor = 0;
float OptionsManager::EffectVolume = 0.0f;
float OptionsManager::MusicVolume = 0.0f;
std::string OptionsManager::Username = "";

void OptionsManager::Load() {
	ACTIASFW_LOG(OptionsManager, Info, "Loading options");
	std::ifstream file("options.txt");
	if (!file.good()) {
		ACTIASFW_LOG(OptionsManager, Warning, "Failed to open options.txt");
		WindowDimensions = glm::ivec2(960, 540);
		Fullscreen = false;
		Monitor = glfwGetPrimaryMonitor();
		EffectVolume = 1.0f;
		MusicVolume = 1.0f;
		Username = "";
		return;
	}
	else {
		std::string windowDimensionsPrefix = "window-dimensions: ";
		bool windowDimensionsSet = false;
		std::string fullscreenPrefix = "fullscreen: ";
		bool fullscreenSet = false;
		std::string effectVolumePrefix = "effect-volume: ";
		bool effectVolumeSet = false;
		std::string musicVolumePrefix = "music-volume: ";
		bool musicVolumeSet = false;
		std::string usernamePrefix = "username: ";
		bool usernameSet = false;
		std::string line;
		while (std::getline(file, line)) {
			if (line.starts_with(windowDimensionsPrefix)) {
				std::string value = line.substr(
					windowDimensionsPrefix.length());
				std::string widthValue = "";
				std::string heightValue = "";
				bool x = false;
				for (int i = 0; i < value.length(); i++) {
					if (value[i] == 'x') {
						x = true;
						continue;
					}
					if (!x) {
						widthValue += value[i];
					}
					else {
						heightValue += value[i];
					}
				}
				if (!x) {
					continue;
				}
				ACTIASFW_LOG(OptionsManager, Info, "Read window dimensions: \"",
					widthValue, "x", heightValue, "\"");
				WindowDimensions = glm::ivec2(std::atoi(widthValue.c_str()),
					std::atoi(heightValue.c_str()));
				windowDimensionsSet = true;
			}
			else if (line.starts_with(fullscreenPrefix)) {
				std::string value = line.substr(fullscreenPrefix.length());
				ACTIASFW_LOG(OptionsManager, Info, "Read fullscreen mode: \"",
					value, "\"");
				Fullscreen = (bool)std::atoi(value.c_str());
				fullscreenSet = true;
			}
			else if (line.starts_with(effectVolumePrefix)) {
				std::string value = line.substr(effectVolumePrefix.length());
				ACTIASFW_LOG(OptionsManager, Info, "Read effect volume: \"",
					value, "\"");
				EffectVolume = (float)std::atof(value.c_str());
				effectVolumeSet = true;
			}
			else if (line.starts_with(musicVolumePrefix)) {
				std::string value = line.substr(musicVolumePrefix.length());
				ACTIASFW_LOG(OptionsManager, Info, "Read music volume: \"",
					value, "\"");
				MusicVolume = (float)std::atof(value.c_str());
				musicVolumeSet = true;
			}
			else if (line.starts_with(usernamePrefix)) {
				std::string value = line.substr(usernamePrefix.length());
				ACTIASFW_LOG(OptionsManager, Info, "Read username: \"", value,
					"\"");
				Username = value;
				usernameSet = true;
			}
		}
		if (!windowDimensionsSet) {
			WindowDimensions = glm::ivec2(960, 540);
		}
		if (!fullscreenSet) {
			Fullscreen = false;
		}
		Monitor = glfwGetPrimaryMonitor();
		if (!effectVolumeSet) {
			EffectVolume = 1.0f;
		}
		if (!musicVolumeSet) {
			MusicVolume = 1.0f;
		}
		if (!usernameSet) {
			Username = "";
		}
	}
	file.close();
}

void OptionsManager::Save() {
	std::ofstream file("options.txt");
	if (!file.good()) {
		return;
	}
	file << "window-dimensions: " << WindowDimensions.x << "x"
		<< WindowDimensions.y << std::endl;
	file << "fullscreen: " << (int)Fullscreen << std::endl;
	file << "effect-volume: " << EffectVolume << std::endl;
	file << "music-volume: " << MusicVolume << std::endl;
	file << "username: " << Username;
	file.close();
}