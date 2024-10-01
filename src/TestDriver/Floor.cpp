/*
File:		Floor.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.19@19:11
Purpose:	Implement functions found in the Floor class of Sprites.h
*/

#include "Sprites.h"

void Floor::initialize(const glm::vec3& position) {
	Sprite::initialize(position, glm::vec2(128.0f, 128.0f),
		"Assets/textures/floor.png");
}