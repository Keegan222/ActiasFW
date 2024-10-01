/*
File:		Player.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.19@20:20
Purpose:	Implement the functions found in the Player class of Sprites.h
*/

#include "TestDriver.h"

void Player::initialize(const glm::vec3& position) {
	Animation::initialize(position, glm::vec2(64.0f, 64.0f),
		"Assets/textures/player.png", glm::ivec2(5, 1),
		(float)Application::Window.getTargetUpdatesPerSecond() / 6.0f);
}

void Player::processInput() {
	if (Application::Input.isKeyDown(KEY_A)
		|| Application::Input.getControllerAxisPosition(CONTROLLER_0,
			CONTROLLER_AXIS_LEFT_STICK_X) < -0.4f) {
		m_velocity.x = -4.0f;
	}
	else if (Application::Input.isKeyDown(KEY_D)
		|| Application::Input.getControllerAxisPosition(CONTROLLER_0,
			CONTROLLER_AXIS_LEFT_STICK_X) > 0.4f) {
		m_velocity.x = 4.0f;
	}
	else {
		m_velocity.x = 0.0f;
	}
	if (Application::Input.isKeyDown(KEY_S)
		|| Application::Input.getControllerAxisPosition(CONTROLLER_0,
			CONTROLLER_AXIS_LEFT_STICK_Y) > 0.4f) {
		m_velocity.y = -4.0f;
	}
	else if (Application::Input.isKeyDown(KEY_W)
		|| Application::Input.getControllerAxisPosition(CONTROLLER_0,
			CONTROLLER_AXIS_LEFT_STICK_Y) < -0.4f) {
		m_velocity.y = 4.0f;
	}
	else {
		m_velocity.y = 0.0f;
	}
}

void Player::update(float timeStep) {
	if (m_velocity.x < 0.0f) {
		m_reflectedHorizontally = false;
	}
	else if (m_velocity.x > 0.0f) {
		m_reflectedHorizontally = true;
	}
	if (m_velocity.x == 0.0f && m_velocity.y == 0.0f && m_playing) {
		stop();
	}
	else {
		play();
	}
	Animation::update(timeStep);
	m_position.x -= m_velocity.x * timeStep;
	m_position.y -= m_velocity.y * timeStep;
	for (Wall& wall : TestDriver::Game.getWalls()) {
		m_position.x += m_velocity.x * timeStep;
		bool xi = wall.intersects(*this);
		m_position.x -= m_velocity.x * timeStep;
		m_position.y += m_velocity.y * timeStep;
		bool yi = wall.intersects(*this);
		m_position.y -= m_velocity.y * timeStep;
		if (xi) {
			if (m_velocity.x < 0.0f) {
				m_position.x = wall.getPosition().x + wall.getDimensions().x
					+ 0.1f;
			}
			else if (m_velocity.x > 0.0f) {
				m_position.x = wall.getPosition().x - m_dimensions.x - 0.1f;
			}
			m_velocity.x = 0.0f;
		}
		if (yi) {
			if (m_velocity.y < 0.0f) {
				m_position.y = wall.getPosition().y + wall.getDimensions().y
					+ 0.1f;
			}
			else if (m_velocity.y > 0.0f) {
				m_position.y = wall.getPosition().y - m_dimensions.y - 0.1f;
			}
			m_velocity.y = 0.0f;
		}
	}
	m_position.x += m_velocity.x * timeStep;
	m_position.y += m_velocity.y * timeStep;
	for (Coin& coin : TestDriver::Game.getCoins()) {
		if (coin.intersects(*this)) {
			Application::Audio.playEffect("Assets/audio/effects/coin.wav");
			coin.setCollected(true);
			TestDriver::Game.collectCoin();
		}
	}
	for (Enemy& enemy : TestDriver::Game.getEnemies()) {
		if (enemy.intersects(*this)) {
			Application::Audio.playEffect("Assets/audio/effects/crunch.wav");
			TestDriver::Game.die();
		}
	}
}

void Player::destroy() {
	Animation::destroy();
}