/*
File:		TestDriver.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.05@14:05
Purpose:	Declare the main class and scene classes for a test of the Actias
			Framework
*/

#ifndef TESTDRIVER_TEST_DRIVER_H
#define TESTDRIVER_TEST_DRIVER_H

#include "Sprites.h"

class TitleScene : public Scene {
private:
	UIGroup m_UI;
	UILabel m_titleLabel;
	UIButton m_startButton;
	UIButton m_optionsButton;
	UIButton m_quitButton;
	bool m_running = false;
	std::vector<Spark> m_sparks;
	float m_timer = 0.0f;

	bool initialize() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void UIEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	bool destroy() override;
};

class OptionsManager {
public:
	static glm::ivec2 WindowDimensions;
	static bool Fullscreen;
	static GLFWmonitor* Monitor;
	static float EffectVolume;
	static float MusicVolume;
	static std::string Username;

	static void Load();
	static void Save();
};

class OptionsScene : public Scene {
private:
	UIGroup m_UI;
	UIButton m_backButton;
	UIButton m_defaultsButton;
	UIButton m_applyButton;
	UICarousel m_windowDimensionsCarousel;
	UISwitch m_fullscreenSwitch;
	UICarousel m_monitorCarousel;
	UISlider m_effectVolumeSlider;
	UISlider m_musicVolumeSlider;
	UITextBox m_usernameBox;
	std::vector<Spark> m_sparks;
	float m_timer = 0.0f;
	Scene* m_lastScene = 0;

	bool initialize() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void UIEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	bool destroy() override;
};

class GameScene : public Scene {
public:
	std::vector<Wall>& getWalls();
	std::vector<Coin>& getCoins();
	void collectCoin();
	std::vector<Enemy>& getEnemies();
	void die();

private:
	std::vector<Floor> m_floors;
	std::vector<Wall> m_walls;
	std::vector<Coin> m_coins;
	std::vector<Enemy> m_enemies;
	Player m_player;
	UIGroup m_usernameUI;
	UILabel m_usernameLabel;
	Camera m_UICamera;
	UIGroup m_coinsUI;
	UILabel m_coinsLabel;
	unsigned int m_coinCount = 0;
	UIGroup m_pauseUI;
	UILabel m_pauseBackground;
	UIButton m_resumeButton;
	UIButton m_restartButton;
	UIButton m_optionsButton;
	UIButton m_quitButton;
	bool m_dead = false;

	bool initialize() override;
	void enter(Scene&) override;
	void draw() override;
	bool processInput() override;
	void UIEvent(unsigned int, unsigned int, unsigned int) override;
	void update(float) override;
	void leave(Scene&) override;
	bool destroy() override;
	bool loadMap();
};

class TestDriver {
public:
	static TitleScene Title;
	static OptionsScene Options;
	static GameScene Game;
};

#endif