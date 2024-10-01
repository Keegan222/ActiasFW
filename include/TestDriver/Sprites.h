/*
File:		Sprites.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.09.18@21:59
Purpose:	Declares classes representing all the entities to appear in the
			TestDriver program game
*/

#include <ActiasFW/ActiasFW.h>

#ifndef TESTDRIVER_SPRITES_H
#define TESTDRIVER_SPRITES_H

using namespace ActiasFW;

class Spark : public Animation {
public:
	void initialize();
	void update(float) override;
	void destroy() override;
	bool isAlive() const;

private:
	float m_lifeTime = 0.0f;
	float m_horizontalVelocityTimer = 0.0f;
};

class Floor : public Sprite {
public:
	void initialize(const glm::vec3&);
};

class Wall : public Sprite {
public:
	void initialize(const glm::vec3&);
};

class Coin : public Animation {
public:
	void initialize(const glm::vec3&);
	void destroy() override;
	bool isCollected() const;
	void setCollected(bool);

private:
	bool m_collected = false;
};

class Enemy : public Animation {
public:
	void initialize(const glm::vec3&);
	void update(float) override;
	void destroy() override;

private:
	float m_velocityTimer = 0.0f;
};

class Player : public Animation {
public:
	void initialize(const glm::vec3&);
	void processInput();
	void update(float) override;
	void destroy() override;
};

#endif