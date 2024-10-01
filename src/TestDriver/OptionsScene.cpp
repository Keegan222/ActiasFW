/*
File:		OptionsScene.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.18@18:22
Purpose:	Implement the functions found in the OptionsScene class of
			TestDriver.h
*/

#include "TestDriver.h"

bool OptionsScene::initialize() {
	ACTIASFW_LOG(OptionsScene, Info, "Initializing");
	m_UI.initialize(*this, 0, "Assets/fonts/open-sans/OpenSans-Regular.ttf",
		0.25f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), CONTROLLER_0,
		"Assets/textures/options-highlight.png", glm::vec2(2.0f, 2.0f));
	m_backButton.initialize(glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec2(0.25f, 0.05f), "Assets/textures/options-button.png",
		"BACK");
	m_backButton.setNeighbourLeft(m_applyButton);
	m_backButton.setNeighbourRight(m_defaultsButton);
	m_backButton.setNeighbourBelow(m_windowDimensionsCarousel);
	m_backButton.setNeighbourAbove(m_musicVolumeSlider);
	m_UI.addComponent(m_backButton);
	m_defaultsButton.initialize(glm::vec3(0.5f - (0.25f / 2.0f), 0.0f, 1.0f),
		glm::vec2(0.25f, 0.05f), "Assets/textures/options-button.png",
		"DEFAULTS");
	m_defaultsButton.setNeighbourLeft(m_backButton);
	m_defaultsButton.setNeighbourRight(m_applyButton);
	m_defaultsButton.setNeighbourBelow(m_windowDimensionsCarousel);
	m_defaultsButton.setNeighbourAbove(m_musicVolumeSlider);
	m_UI.addComponent(m_defaultsButton);
	m_applyButton.initialize(glm::vec3(0.75f, 0.0f, 1.0f),
		glm::vec2(0.25f, 0.05f), "Assets/textures/options-button.png",
		"APPLY");
	m_applyButton.setNeighbourLeft(m_defaultsButton);
	m_applyButton.setNeighbourRight(m_backButton);
	m_applyButton.setNeighbourBelow(m_usernameBox);
	m_applyButton.setNeighbourAbove(m_usernameBox);
	m_UI.addComponent(m_applyButton);
	m_windowDimensionsCarousel.initialize(glm::vec3(0.05f, 0.9f, 1.0f),
		glm::vec2(0.4f, 0.05f), "Assets/textures/options-carousel.png",
		"WINDOW RESOLUTIONS", { "960x540", "1280x720", "1920x1080", },
		"960x540", 0.1f, "Assets/textures/options-carousel-button.png");
	m_windowDimensionsCarousel.setNeighbourLeft(m_usernameBox);
	m_windowDimensionsCarousel.setNeighbourRight(m_usernameBox);
	m_windowDimensionsCarousel.setNeighbourBelow(m_fullscreenSwitch);
	m_windowDimensionsCarousel.setNeighbourAbove(m_backButton);
	m_UI.addComponent(m_windowDimensionsCarousel);
	m_fullscreenSwitch.initialize(glm::vec3(0.05f, 0.8f, 1.0f),
		glm::vec2(0.1f, 0.05f), "Assets/textures/options-switch.png",
		"FULLSCREEN", ALIGN_TOP, ALIGN_LEFT, ALIGN_BOTTOM);
	m_fullscreenSwitch.setNeighbourLeft(m_usernameBox);
	m_fullscreenSwitch.setNeighbourRight(m_usernameBox);
	m_fullscreenSwitch.setNeighbourBelow(m_monitorCarousel);
	m_fullscreenSwitch.setNeighbourAbove(m_windowDimensionsCarousel);
	m_UI.addComponent(m_fullscreenSwitch);
	m_monitorCarousel.initialize(glm::vec3(0.05f, 0.7f, 1.0f),
		glm::vec2(0.4f, 0.05f), "Assets/textures/options-carousel.png",
		"MONITOR", { }, "", 0.1f,
		"Assets/textures/options-carousel-button.png");
	m_monitorCarousel.setNeighbourLeft(m_usernameBox);
	m_monitorCarousel.setNeighbourRight(m_usernameBox);
	m_monitorCarousel.setNeighbourBelow(m_effectVolumeSlider);
	m_monitorCarousel.setNeighbourAbove(m_fullscreenSwitch);
	m_UI.addComponent(m_monitorCarousel);
	m_effectVolumeSlider.initialize(glm::vec3(0.05f, 0.6f, 1.0f),
		glm::vec2(0.4f, 0.05f), "Assets/textures/options-slider.png",
		glm::vec2(2.0f, 25.0f), "Assets/textures/options-cursor.png",
		"SOUND EFFECT VOLUME", 0.0f);
	m_effectVolumeSlider.setNeighbourLeft(m_usernameBox);
	m_effectVolumeSlider.setNeighbourRight(m_usernameBox);
	m_effectVolumeSlider.setNeighbourBelow(m_musicVolumeSlider);
	m_effectVolumeSlider.setNeighbourAbove(m_monitorCarousel);
	m_UI.addComponent(m_effectVolumeSlider);
	m_musicVolumeSlider.initialize(glm::vec3(0.05f, 0.5f, 1.0f),
		glm::vec2(0.4f, 0.05f), "Assets/textures/options-slider.png",
		glm::vec2(2.0f, 25.0f), "Assets/textures/options-cursor.png",
		"MUSIC VOLUME", 0.0f);
	m_musicVolumeSlider.setNeighbourLeft(m_usernameBox);
	m_musicVolumeSlider.setNeighbourRight(m_usernameBox);
	m_musicVolumeSlider.setNeighbourBelow(m_backButton);
	m_musicVolumeSlider.setNeighbourAbove(m_effectVolumeSlider);
	m_UI.addComponent(m_musicVolumeSlider);
	m_usernameBox.initialize(glm::vec3(0.55f, 0.9f, 1.0f),
		glm::vec2(0.4f, 0.05f), "Assets/textures/options-text-box.png",
		glm::vec2(2.0f, 25.0f), "Assets/textures/options-cursor.png",
		"USERNAME", "");
	m_usernameBox.setNeighbourLeft(m_windowDimensionsCarousel);
	m_usernameBox.setNeighbourRight(m_windowDimensionsCarousel);
	m_usernameBox.setNeighbourBelow(m_applyButton);
	m_usernameBox.setNeighbourAbove(m_applyButton);
	m_UI.addComponent(m_usernameBox);
	return true;
}

void OptionsScene::enter(Scene& lastScene) {
	ACTIASFW_LOG(OptionsScene, Info, "Entering from ", &lastScene);
	Application::Audio.playMusic("Assets/audio/music/waiting.wav");
	Application::Graphics.getDefaultCamera().setPosition(glm::vec3(
		(float)Application::Window.getDimensions().x / 2.0f,
		(float)Application::Window.getDimensions().y / 2.0f, 0.0f));
	Application::Graphics.getDefaultCamera().setScale(1.0f);
	m_windowDimensionsCarousel.setCurrentValue(
		std::to_string(OptionsManager::WindowDimensions.x) + "x"
		+ std::to_string(OptionsManager::WindowDimensions.y));
	m_fullscreenSwitch.setOn(OptionsManager::Fullscreen);
	int monitorCount = 0;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	m_monitorCarousel.clear();
	for (int i = 0; i < monitorCount; i++) {
		m_monitorCarousel.addValue(std::to_string(i) + ": "
			+ glfwGetMonitorName(monitors[i]));
	}
	m_effectVolumeSlider.setValue(OptionsManager::EffectVolume);
	m_musicVolumeSlider.setValue(OptionsManager::MusicVolume);
	m_usernameBox.setText(OptionsManager::Username);
	Application::Graphics.getDefaultCamera().setPosition(glm::vec3(
		(float)Application::Window.getDimensions().x / 2.0f,
		(float)Application::Window.getDimensions().y / 2.0f, 0.0f));
	for (Spark& s : m_sparks) {
		s.destroy();
	}
	m_sparks.clear();
	m_timer = 0.0f;
	m_lastScene = &lastScene;
}

void OptionsScene::draw() {
	m_UI.draw();
	Application::Graphics.submit(m_sparks);
}

bool OptionsScene::processInput() {
	m_UI.processInput();
	return true;
}

void OptionsScene::UIEvent(unsigned int groupID, unsigned int componentID,
	unsigned int eventID) {
	if (groupID == m_UI.getID()) {
		if (componentID == m_backButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				Application::SetCurrentScene(*m_lastScene);
			}
		}
		else if (componentID == m_defaultsButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				m_windowDimensionsCarousel.setCurrentValue("960x540");
				m_fullscreenSwitch.setOn(false);
				m_monitorCarousel.setCurrentValue(
					glfwGetMonitorName(glfwGetPrimaryMonitor()));
				m_effectVolumeSlider.setValue(1.0f);
				m_musicVolumeSlider.setValue(1.0f);
				m_usernameBox.setText("");
			}
		}
		else if (componentID == m_applyButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				ACTIASFW_LOG(OptionsScene, Info, "Apply clicked");
				std::string windowDimensionsStr
					= m_windowDimensionsCarousel.getCurrentValue();
				std::string windowWidthStr = "";
				std::string windowHeightStr = "";
				bool x = false;
				for (size_t i = 0; i < windowDimensionsStr.length(); i++) {
					if (windowDimensionsStr[i] == 'x') {
						x = true;
						continue;
					}
					if (!x) {
						windowWidthStr += windowDimensionsStr[i];
					}
					else {
						windowHeightStr += windowDimensionsStr[i];
					}
				}
				glm::ivec2 windowDimensions(std::atoi(windowWidthStr.c_str()),
					std::atoi(windowHeightStr.c_str()));
				OptionsManager::WindowDimensions = windowDimensions;
				OptionsManager::Fullscreen = m_fullscreenSwitch.isOn();
				int monitorIndex = std::atoi(
					m_monitorCarousel.getCurrentValue().c_str());
				int monitorCount = 0;
				GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
				if (monitorIndex >= 0 && monitorIndex < monitorCount) {
					OptionsManager::Monitor = monitors[monitorIndex];
				}
				OptionsManager::EffectVolume = m_effectVolumeSlider.getValue();
				OptionsManager::MusicVolume = m_musicVolumeSlider.getValue();
				OptionsManager::Username = m_usernameBox.getText();
				OptionsManager::Save();
				Application::Window.setDimensions(
					OptionsManager::WindowDimensions);
				Application::Window.setMonitor(OptionsManager::Monitor);
				Application::Window.setFullscreen(OptionsManager::Fullscreen);
				Application::Audio.setEffectVolume(
					OptionsManager::EffectVolume);
				Application::Audio.setMusicVolume(OptionsManager::MusicVolume);
			}
		}
	}
}

void OptionsScene::update(float timeStep) {
	m_UI.update(timeStep);
	for (Spark& s : m_sparks) {
		s.update(timeStep);
	}
	m_timer += timeStep;
	if (m_timer > (float)Application::Window
		.getTargetUpdatesPerSecond() / 50.0f) {
		m_sparks.push_back(Spark());
		m_sparks.back().initialize();
		m_timer = 0.0f;
	}
	std::vector<Spark>::iterator it = m_sparks.begin();
	while (it != m_sparks.end()) {
		if (!it->isAlive()) {
			it->destroy();
			it = m_sparks.erase(it);
		}
		else {
			it++;
		}
	}
}

void OptionsScene::leave(Scene& nextScene) {
	ACTIASFW_LOG(OptionsScene, Info, "Leaving for ", &nextScene);
}

bool OptionsScene::destroy() {
	ACTIASFW_LOG(OptionsScene, Info, "Destroying");
	m_UI.destroy();
	for (Spark& s : m_sparks) {
		s.destroy();
	}
	m_sparks.clear();
	m_timer = 0.0f;
	return true;
}