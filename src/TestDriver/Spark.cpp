/*
File:		Spark.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.18@22:03
Purpose:	Implement functions found in the Spark class of Sprites.h
*/

#include "Sprites.h"

void Spark::initialize() {
	m_lifeTime = (float)(3 * Application::Window.getTargetUpdatesPerSecond())
		+ (float)(rand() % 160);
	float dim = 5.0f + (float)(rand() % 6);
	glm::vec2 dims(dim, dim);
	glm::vec3 pos(
		(float)(rand() % (Application::Window.getDimensions().x - (int)dims.x)),
		-10.0f, 0.0f);
	Animation::initialize(pos, dims, "Assets/textures/spark.png",
		glm::ivec2(5, 1), m_lifeTime / 5.0f);
	m_velocity.x = (-1.0f + (rand() % 2 == 0 ? 0.0f : 2.0f))
		* (3.0f * (float)(rand() % 2) / 2.0f);
	m_velocity.y = 4.0f + (float)(rand() % 5);
	play();
}

void Spark::update(float timeStep) {
	Animation::update(timeStep);
	m_lifeTime -= timeStep;
	m_horizontalVelocityTimer += timeStep;
	if (m_horizontalVelocityTimer
		> (float)Application::Window.getTargetUpdatesPerSecond()) {
		m_horizontalVelocityTimer = 0.0f;
		m_velocity.x = -m_velocity.x;
	}
}

void Spark::destroy() {
	Animation::destroy();
	m_lifeTime = 0.0f;
}

bool Spark::isAlive() const {
	return m_lifeTime > 0.0f;
}