/*
File:		Wall.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.19@19:13
Purpose:	Implements functions found in the Wall class of Sprites.h
*/

#include "Sprites.h"

void Wall::initialize(const glm::vec3& position) {
	Sprite::initialize(position, glm::vec2(128.0f, 128.0f),
		"Assets/textures/wall.png");
}