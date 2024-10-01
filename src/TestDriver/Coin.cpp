/*
File:		Coin.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.19@19:15
Purpose:	Implements the functions found in the Coin class of Sprites.h
*/

#include "Sprites.h"

void Coin::initialize(const glm::vec3& position) {
	Animation::initialize(position, glm::vec2(64.0f, 64.0f),
		"Assets/textures/coin.png", glm::ivec2(8, 1),
		(float)Application::Window.getTargetUpdatesPerSecond() / 4.0f);
	setCollected(false);
	play();
}

void Coin::destroy() {
	Animation::destroy();
	m_collected = false;
}

bool Coin::isCollected() const {
	return m_collected;
}

void Coin::setCollected(bool collected) {
	m_collected = collected;
}