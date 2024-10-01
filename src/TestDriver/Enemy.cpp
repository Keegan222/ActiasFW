/*
File:		Enemy.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.19@19:17
Purpose:	Implement the functions found in the Enemy class of Sprites.h
*/

#include "TestDriver.h"

void Enemy::initialize(const glm::vec3& position) {
	Animation::initialize(position, glm::vec2(64.0f, 64.0f),
		"Assets/textures/enemy.png", glm::ivec2(5, 1),
		(float)Application::Window.getTargetUpdatesPerSecond() / 4.0f);
}

void Enemy::update(float timeStep) {
	m_velocityTimer += timeStep;
	if (m_velocityTimer > (float)Application::Window
		.getTargetUpdatesPerSecond()) {
		float vx = (rand() % 2 == 0 ? -1.0f : 1.0f)
			* ((float)(rand() % 6) / 3.0f);
		float vy = (rand() % 2 == 0 ? -1.0f : 1.0f)
			* ((float)(rand() % 6) / 3.0f);
		m_velocity = glm::vec2(vx, vy);
		m_velocityTimer = 0.0f;
	}
	m_reflectedHorizontally = (m_velocity.x > 0.0f);
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
}

void Enemy::destroy() {
	Animation::destroy();
	m_velocityTimer = 0.0f;
}