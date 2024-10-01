/*
File:		TitleScene.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.18@18:18
Purpose:	Implement the functions found in the TitleScene class of
			TestDriver.h
*/

#include "TestDriver.h"

bool TitleScene::initialize() {
	ACTIASFW_LOG(TitleScene, Info, "Initializing");
	m_UI.initialize(*this, 0, "Assets/fonts/open-sans/OpenSans-Regular.ttf",
		0.5f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), CONTROLLER_0,
		"Assets/textures/title-highlight.png", glm::vec2(2.0f, 2.0f));
	m_titleLabel.initialize(glm::vec3(0.0f, 0.7f, 1.0f),
		glm::vec2(1.0f, 0.15f), "", "FN STUDIOS PRESENTS: ACTIAS TEST DRIVER");
	m_UI.addComponent(m_titleLabel);
	m_startButton.initialize(glm::vec3(0.25f, 0.6f, 1.0f),
		glm::vec2(0.5f, 0.1f), "Assets/textures/title-button.png", "START");
	m_startButton.setNeighbourBelow(m_optionsButton);
	m_startButton.setNeighbourAbove(m_quitButton);
	m_UI.addComponent(m_startButton);
	m_optionsButton.initialize(glm::vec3(0.25f, 0.6f - 0.2f, 1.0f),
		glm::vec2(0.5f, 0.1f), "Assets/textures/title-button.png",
		"OPTIONS");
	m_optionsButton.setNeighbourBelow(m_quitButton);
	m_optionsButton.setNeighbourAbove(m_startButton);
	m_UI.addComponent(m_optionsButton);
	m_quitButton.initialize(glm::vec3(0.25f, 0.6f - 0.4f, 1.0f),
		glm::vec2(0.5f, 0.1f), "Assets/textures/title-button.png", "QUIT");
	m_quitButton.setNeighbourBelow(m_startButton);
	m_quitButton.setNeighbourAbove(m_optionsButton);
	m_UI.addComponent(m_quitButton);
	m_UI.setInitialComponent(m_startButton);
	m_running = true;
	return true;
}

void TitleScene::enter(Scene& lastScene) {
	ACTIASFW_LOG(TitleScene, Info, "Entering from ", &lastScene);
	Application::Audio.playMusic("Assets/audio/music/waiting.wav");
	Application::Graphics.getDefaultCamera().setPosition(glm::vec3(
		(float)Application::Window.getDimensions().x / 2.0f,
		(float)Application::Window.getDimensions().y / 2.0f, 0.0f));
	Application::Graphics.getDefaultCamera().setScale(1.0f);
	for (Spark& s : m_sparks) {
		s.destroy();
	}
	m_sparks.clear();
	m_timer = 0.0f;
}

void TitleScene::draw() {
	m_UI.draw();
	Application::Graphics.submit(m_sparks);
}

bool TitleScene::processInput() {
	m_UI.processInput();
	return m_running;
}

void TitleScene::UIEvent(unsigned int groupID, unsigned int componentID,
	unsigned int eventID) {
	if (groupID == m_UI.getID()) {
		if (componentID == m_startButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				Application::SetCurrentScene(TestDriver::Game);
			}
		}
		else if (componentID == m_optionsButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				Application::SetCurrentScene(TestDriver::Options);
			}
		}
		else if (componentID == m_quitButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				m_running = false;
			}
		}
	}
}

void TitleScene::update(float timeStep) {
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

void TitleScene::leave(Scene& nextScene) {
	ACTIASFW_LOG(TitleScene, Info, "Leaving for ", &nextScene);
}

bool TitleScene::destroy() {
	ACTIASFW_LOG(TitleScene, Info, "Destroying");
	m_UI.destroy();
	m_running = false;
	for (Spark& s : m_sparks) {
		s.destroy();
	}
	m_sparks.clear();
	m_timer = 0.0f;
	return true;
}