/*
File:		GameScene.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.18@18:22
Purpose:	Implement the functions found in the GameScene class of TestDriver.h
*/

#include "TestDriver.h"

std::vector<Wall>& GameScene::getWalls() {
	return m_walls;
}

std::vector<Coin>& GameScene::getCoins() {
	return m_coins;
}

void GameScene::collectCoin() {
	m_coinCount++;
}

std::vector<Enemy>& GameScene::getEnemies() {
	return m_enemies;
}

void GameScene::die() {
	m_pauseUI.setEnabled(true);
	m_pauseUI.setVisible(true);
	m_resumeButton.setEnabled(false);
	m_resumeButton.setVisible(false);
	m_restartButton.setNeighbourAbove(m_quitButton);
	m_quitButton.setNeighbourBelow(m_restartButton);
	m_resumeButton.setHighlighted(false);
	m_pauseUI.setInitialComponent(m_restartButton);
	m_dead = true;
}

bool GameScene::initialize() {
	ACTIASFW_LOG(GameScene, Info, "Initializing");
	m_usernameUI.initialize(*this, 0,
		"Assets/fonts/open-sans/OpenSans-regular.ttf", 0.15f,
		glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), CONTROLLER_UNKNOWN, "", glm::vec2());
	m_usernameLabel.initialize(glm::vec3(), glm::vec2(1.0f, 1.0f),
		"", "", ALIGN_CENTER, ALIGN_CENTER, ALIGN_BOTTOM);
	m_usernameLabel.getBackgroundSprite().setColor(
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	m_usernameUI.addComponent(m_usernameLabel);
	m_UICamera.initialize();
	m_UICamera.setPosition(glm::vec3(
		(float)Application::Window.getDimensions().x / 2.0f,
		(float)Application::Window.getDimensions().y / 2.0f, 0.0f));
	m_coinsUI.initialize(*this, 1,
		"Assets/fonts/open-sans/OpenSans-Regular.ttf", 0.5f,
		glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), CONTROLLER_UNKNOWN, "", glm::vec2());
	m_coinsUI.getShader().setCamera(m_UICamera);
	m_coinsLabel.initialize(glm::vec3(0.0f, 0.9f, 1.1f), glm::vec2(0.5f, 0.1f),
		"", "", ALIGN_CENTER, ALIGN_LEFT, ALIGN_CENTER);
	m_coinsLabel.getBackgroundSprite().setColor(
		glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
	m_coinsUI.addComponent(m_coinsLabel);
	m_coinCount = 0;
	m_pauseUI.initialize(*this, 2,
		"Assets/fonts/open-sans/OpenSans-Regular.ttf", 0.25f,
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), CONTROLLER_0,
		"Assets/textures/game-highlight.png", glm::vec2(2.0f, 2.0f));
	m_pauseUI.getShader().setCamera(m_UICamera);
	m_pauseBackground.initialize(glm::vec3(0.34f, 0.29f, 1.9f),
		glm::vec2(0.32f, 0.37f), "Assets/textures/game-label.png", "");
	m_pauseUI.addComponent(m_pauseBackground);
	m_resumeButton.initialize(glm::vec3(0.35f, 0.6f, 2.0f),
		glm::vec2(0.3f, 0.05f), "Assets/textures/game-button.png", "RESUME");
	m_resumeButton.setNeighbourBelow(m_restartButton);
	m_resumeButton.setNeighbourAbove(m_quitButton);
	m_pauseUI.addComponent(m_resumeButton);
	m_restartButton.initialize(glm::vec3(0.35f, 0.5f, 2.0f),
		glm::vec2(0.3f, 0.05f), "Assets/textures/game-button.png", "RESTART");
	m_restartButton.setNeighbourBelow(m_optionsButton);
	m_restartButton.setNeighbourAbove(m_resumeButton);
	m_pauseUI.addComponent(m_restartButton);
	m_optionsButton.initialize(glm::vec3(0.35f, 0.4f, 2.0f),
		glm::vec2(0.3f, 0.05f), "Assets/textures/game-button.png", "OPTIONS");
	m_optionsButton.setNeighbourBelow(m_quitButton);
	m_optionsButton.setNeighbourAbove(m_restartButton);
	m_pauseUI.addComponent(m_optionsButton);
	m_quitButton.initialize(glm::vec3(0.35f, 0.3f, 2.0f),
		glm::vec2(0.3f, 0.05f), "Assets/textures/game-button.png", "QUIT");
	m_quitButton.setNeighbourBelow(m_resumeButton);
	m_quitButton.setNeighbourAbove(m_optionsButton);
	m_pauseUI.addComponent(m_quitButton);
	m_pauseUI.setInitialComponent(m_resumeButton);
	m_pauseUI.setEnabled(false);
	m_pauseUI.setVisible(false);
	m_dead = false;
	return loadMap();
}

void GameScene::enter(Scene& lastScene) {
	ACTIASFW_LOG(GameScene, Info, "Entering from ", &lastScene);
	Application::Audio.playMusic("Assets/audio/music/playing.wav");
	Application::Graphics.getDefaultCamera().setScale(2.0f);
	m_usernameLabel.setLabelText(OptionsManager::Username);
}

void GameScene::draw() {
	Application::Graphics.submit(m_floors);
	Application::Graphics.submit(m_walls);
	Application::Graphics.submit(m_coins);
	Application::Graphics.submit(m_enemies);
	Application::Graphics.submit(m_player);
	m_usernameUI.draw();
	m_coinsUI.draw();
	m_pauseUI.draw();
}

bool GameScene::processInput() {
	if (!m_pauseUI.isEnabled()) {
		m_player.processInput();
	}
	m_pauseUI.processInput();
	if ((Application::Input.isKeyPressed(KEY_ESCAPE)
		|| Application::Input.isControllerButtonPressed(CONTROLLER_0,
			CONTROLLER_BUTTON_START)) && !m_dead) {
		m_pauseUI.setEnabled(!m_pauseUI.isEnabled());
		m_pauseUI.setVisible(!m_pauseUI.isVisible());
	}
	return true;
}

void GameScene::UIEvent(unsigned int groupID, unsigned int componentID,
	unsigned int eventID) {
	if (groupID == m_pauseUI.getID()) {
		if (componentID == m_resumeButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				m_pauseUI.setEnabled(false);
				m_pauseUI.setVisible(false);
			}
		}
		else if (componentID == m_restartButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				Application::Audio.playEffect("Assets/audio/effects/click.wav");
				for (Floor& floor : m_floors) {
					floor.destroy();
				}
				m_floors.clear();
				for (Wall& wall : m_walls) {
					wall.destroy();
				}
				m_walls.clear();
				for (Coin& coin : m_coins) {
					coin.destroy();
				}
				m_coins.clear();
				m_coinCount = 0;
				for (Enemy& enemy : m_enemies) {
					enemy.destroy();
				}
				m_enemies.clear();
				m_player.destroy();
				m_dead = false;
				loadMap();
				m_pauseUI.setEnabled(false);
				m_pauseUI.setVisible(false);
				m_restartButton.setNeighbourAbove(m_resumeButton);
				m_quitButton.setNeighbourBelow(m_resumeButton);
				m_pauseUI.setInitialComponent(m_resumeButton);
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
				UIEvent(m_pauseUI.getID(), m_restartButton.getID(),
					UIButton::ClickedEventID);
				Application::SetCurrentScene(TestDriver::Title);
			}
		}
	}
}

void GameScene::update(float timeStep) {
	if (!m_pauseUI.isEnabled()) {
		for (Coin& coin : m_coins) {
			coin.update(timeStep);
		}
		std::vector<Coin>::iterator it = m_coins.begin();
		while (it != m_coins.end()) {
			if (it->isCollected()) {
				it->destroy();
				it = m_coins.erase(it);
			}
			else {
				it++;
			}
		}
		if (m_coins.empty() && !m_dead) {
			Application::Audio.playEffect("Assets/audio/effects/trumpet.wav");
			die();
		}
		for (Enemy& enemy : m_enemies) {
			enemy.update(timeStep);
		}
		m_player.update(timeStep);
	}
	Application::Graphics.getDefaultCamera().setPosition(m_player.getPosition()
		+ glm::vec3(32.0f, 32.0f, 0.0f));
	glm::vec3 upos(
		(m_player.getPosition().x + (m_player.getDimensions().x / 2.0f))
		- ((float)Application::Window.getDimensions().x / 2.0f),
		m_player.getPosition().y + m_player.getDimensions().y, 1.0f);
	upos.x /= (float)Application::Window.getDimensions().x;
	upos.y /= (float)Application::Window.getDimensions().y;
	m_usernameLabel.setPosition(upos);
	m_usernameLabel.setDimensions(glm::vec2(1.0f, 1.0f));
	m_UICamera.update(timeStep);
	m_coinsLabel.setLabelText("Coins: " + std::to_string(m_coinCount));
	m_coinsLabel.setPosition(glm::vec3(0.0f, 0.9f, 1.1f));
	m_coinsLabel.setDimensions(glm::vec2(0.5f, 0.1f));
	m_pauseUI.update(timeStep);
}

void GameScene::leave(Scene& nextScene) {
	ACTIASFW_LOG(GameScene, Info, "Leaving for ", &nextScene);
}

bool GameScene::destroy() {
	ACTIASFW_LOG(GameScene, Info, "Destroying");
	for (Floor& floor : m_floors) {
		floor.destroy();
	}
	m_floors.clear();
	for (Wall& wall : m_walls) {
		wall.destroy();
	}
	m_walls.clear();
	for (Coin& coin : m_coins) {
		coin.destroy();
	}
	m_coins.clear();
	for (Enemy& enemy : m_enemies) {
		enemy.destroy();
	}
	m_enemies.clear();
	m_player.destroy();
	m_usernameUI.destroy();
	m_UICamera.destroy();
	m_coinsUI.destroy();
	m_coinCount = 0;
	m_pauseUI.destroy();
	return true;
}

bool GameScene::loadMap() {
	ACTIASFW_LOG(GameScene, Info, "Loading map");
	std::ifstream mapFile("Assets/map.txt");
	if (!mapFile.good()) {
		ACTIASFW_LOG(GameScene, Error, "Failed to load map file");
		return false;
	}
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(mapFile, line)) {
		lines.push_back(line);
	}
	mapFile.close();
	for (size_t ly = 0; ly < lines.size(); ly++) {
		for (size_t lx = 0; lx < lines[ly].length(); lx++) {
			float ty = (float)(lines.size() - 1 - ly) * 128.0f;
			float tx = (float)lx * 128.0f;
			char tc = lines[ly][lx];
			m_floors.push_back(Floor());
			m_floors.back().initialize(glm::vec3(tx, ty, 0.0f));
			if (tc == '#') {
				m_walls.push_back(Wall());
				m_walls.back().initialize(glm::vec3(tx, ty, 0.1f));
			}
			else if (tc == '@') {
				m_coins.push_back(Coin());
				m_coins.back().initialize(
					glm::vec3(tx + 32.0f, ty + 32.0f, 0.2f));
			}
			else if (tc == '!') {
				m_enemies.push_back(Enemy());
				m_enemies.back().initialize(
					glm::vec3(tx + 32.0f, ty + 32.0f, 0.3f));
			}
			else if (tc == 'P') {
				m_player.initialize(glm::vec3(tx + 32.0f, ty + 32.0f, 0.4f));
			}
		}
	}
	return true;
}