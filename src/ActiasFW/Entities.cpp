/*
File:		Entities.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.16@15:02
Purpose:	Define variables and implement functions found in Entities.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement Entity class functions

	void Entity::initialize(const glm::vec3& position,
		const glm::vec2& velocity) {
		setPosition(position);
		setVelocity(velocity);
	}

	void Entity::update(float timeStep) {
		m_position.x += m_velocity.x * timeStep;
		m_position.y += m_velocity.y * timeStep;
	}

	void Entity::destroy() {
		m_position = glm::vec3();
		m_velocity = glm::vec2();
	}

	const glm::vec3& Entity::getPosition() const {
		return m_position;
	}

	void Entity::setPosition(const glm::vec3& position) {
		m_position = position;
	}

	const glm::vec2& Entity::getVelocity() const {
		return m_velocity;
	}

	void Entity::setVelocity(const glm::vec2& velocity) {
		m_velocity = velocity;
	}

	// Implement Sprite class functions

	void Sprite::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName) {
		Entity::initialize(position, glm::vec2());
		setDimensions(dimensions);
		setRotation(0.0f);
		setRotationVelocity(0.0f);
		setTextureFileName(textureFileName);
		setTextureBox(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		setColor(glm::vec4());
		setReflectedHorizontally(false);
		setReflectedVertically(false);
		m_indices.clear();
		m_vertices.clear();
	}

	void Sprite::update(float timeStep) {
		Entity::update(timeStep);
		m_rotation += m_rotationVelocity * timeStep;
	}

	bool Sprite::intersects(const Sprite& sprite) {
		if (m_position.x > sprite.m_position.x + sprite.m_dimensions.x
			|| m_position.x + m_dimensions.x < sprite.m_position.x) {
			return false;
		}
		if (m_position.y > sprite.m_position.y + sprite.m_dimensions.y
			|| m_position.y + m_dimensions.y < sprite.m_position.y) {
			return false;
		}
		return true;
	}

	void Sprite::destroy() {
		Entity::destroy();
		m_dimensions = glm::vec2();
		m_rotation = 0.0f;
		m_rotationVelocity = 0.0f;
		m_textureFileName = "";
		m_textureBox = glm::vec4();
		m_color = glm::vec4();
		m_reflectedHorizontally = false;
		m_reflectedVertically = false;
		m_indices.clear();
		m_vertices.clear();
	}

	const glm::vec2& Sprite::getDimensions() const {
		return m_dimensions;
	}

	void Sprite::setDimensions(const glm::vec2& dimensions) {
		m_dimensions = dimensions;
	}

	float Sprite::getRotation() const {
		return m_rotation;
	}

	void Sprite::setRotation(float rotation) {
		m_rotation = rotation;
	}

	float Sprite::getRotationVelocity() const {
		return m_rotationVelocity;
	}

	void Sprite::setRotationVelocity(float rotationVelocity) {
		m_rotationVelocity = rotationVelocity;
	}

	const std::string& Sprite::getTextureFileName() const {
		return m_textureFileName;
	}

	void Sprite::setTextureFileName(const std::string& textureFileName) {
		m_textureFileName = textureFileName;
	}

	const glm::vec4& Sprite::getTextureBox() const {
		return m_textureBox;
	}

	void Sprite::setTextureBox(const glm::vec4& textureBox) {
		m_textureBox = textureBox;
	}

	const glm::vec4& Sprite::getColor() const {
		return m_color;
	}

	void Sprite::setColor(const glm::vec4& color) {
		m_color = color;
	}

	bool Sprite::isReflectedHorizontally() const {
		return m_reflectedHorizontally;
	}

	void Sprite::setReflectedHorizontally(bool reflectedHorizontally) {
		m_reflectedHorizontally = reflectedHorizontally;
	}

	bool Sprite::isReflectedVertically() const {
		return m_reflectedVertically;
	}

	void Sprite::setReflectedVertically(bool reflectedVertically) {
		m_reflectedVertically = reflectedVertically;
	}

	const std::vector<unsigned int>& Sprite::getIndices() const {
		return m_indices;
	}

	const std::vector<float>& Sprite::getVertices() const {
		return m_vertices;
	}

	void Sprite::draw() {
		glm::vec3 bbl(m_position.x, m_position.y, m_position.z);
		glm::vec3 bbr(m_position.x + m_dimensions.x, m_position.y,
			m_position.z);
		glm::vec3 btl(m_position.x, m_position.y + m_dimensions.y,
			m_position.z);
		glm::vec3 btr(m_position.x + m_dimensions.x,
			m_position.y + m_dimensions.y, m_position.z);
		if ((int)m_rotation % 360 != 0) {
			bbl = rotate(bbl);
			bbr = rotate(bbr);
			btl = rotate(btl);
			btr = rotate(btr);
		}
		glm::vec2 tbl(m_textureBox.x, m_textureBox.y + m_textureBox.w);
		glm::vec2 tbr(m_textureBox.x + m_textureBox.z,
			m_textureBox.y + m_textureBox.w);
		glm::vec2 ttl(m_textureBox.x, m_textureBox.y);
		glm::vec2 ttr(m_textureBox.x + m_textureBox.z, m_textureBox.y);
		m_indices = { 0, 2, 1, 1, 2, 3, };
		m_vertices = {
			bbl.x, bbl.y, bbl.z,
			tbl.x, tbl.y,
			m_color.r, m_color.g, m_color.b, m_color.a,
			(float)m_textureFileName.empty(),
			bbr.x, bbr.y, bbr.z,
			tbr.x, tbr.y,
			m_color.r, m_color.g, m_color.b, m_color.a,
			(float)m_textureFileName.empty(),
			btl.x, btl.y, btl.z,
			ttl.x, ttl.y,
			m_color.r, m_color.g, m_color.b, m_color.a,
			(float)m_textureFileName.empty(),
			btr.x, btr.y, btr.z,
			ttr.x, ttr.y,
			m_color.r, m_color.g, m_color.b, m_color.a,
			(float)m_textureFileName.empty(),
		};
		if (m_reflectedHorizontally) {
			transposeVertices(3, 13);
			transposeVertices(4, 14);
			transposeVertices(23, 33);
			transposeVertices(24, 34);
		}
		if (m_reflectedVertically) {
			transposeVertices(3, 23);
			transposeVertices(4, 24);
			transposeVertices(13, 33);
			transposeVertices(14, 34);
		}
	}

#define PI 3.1415926f

	glm::vec3 Sprite::rotate(const glm::vec3& p) const {
		float a = (PI / 180.0f) * m_rotation;
		glm::vec2 c = glm::vec2(m_position.x, m_position.y)
			+ (m_dimensions / 2.0f);
		float rx = cos(a) * (p.x - c.x) - sin(a) * (p.y - c.y) + c.x;
		float ry = sin(a) * (p.x - c.x) + cos(a) * (p.y - c.y) + c.y;
		return glm::vec3(rx, ry, p.z);
	}

	void Sprite::transposeVertices(size_t a, size_t b) {
		float v = m_vertices.at(a);
		m_vertices[a] = m_vertices.at(b);
		m_vertices[b] = v;
	}

	// Implement Animation class functions

	void Animation::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const glm::ivec2& frameDimensions, float frameTime) {
		Sprite::initialize(position, dimensions, textureFileName);
		m_textureBox = glm::vec4(0.0f, 0.0f, 1.0f / (float)frameDimensions.x,
			1.0f / (float)frameDimensions.y);
		m_frameCoordinates.resize(frameDimensions.x * frameDimensions.y);
		for (int y = 0; y < frameDimensions.y; y++) {
			for (int x = 0; x < frameDimensions.x; x++) {
				m_frameCoordinates[x + y * frameDimensions.x]
					= glm::vec2(m_textureBox.z * x, m_textureBox.w * y);
			}
		}
		setCurrentFrame(0);
		setFrameTime(frameTime);
	}

	void Animation::update(float timeStep) {
		Sprite::update(timeStep);
		if (!m_playing) {
			return;
		}
		if (m_timer > m_frameTime) {
			m_timer = 0.0f;
			m_currentFrame = (m_currentFrame + 1) % m_frameCoordinates.size();
		}
		m_timer += timeStep;
		m_textureBox.x = m_frameCoordinates.at(m_currentFrame).x;
		m_textureBox.y = m_frameCoordinates.at(m_currentFrame).y;
	}

	void Animation::play() {
		m_playing = true;
	}

	void Animation::pause() {
		m_playing = false;
	}

	void Animation::stop() {
		m_currentFrame = 0;
		m_timer = 0.0f;
		m_playing = false;
		m_textureBox.x = m_frameCoordinates.at(m_currentFrame).x;
		m_textureBox.y = m_frameCoordinates.at(m_currentFrame).y;
	}

	void Animation::destroy() {
		Sprite::destroy();
		m_frameCoordinates.clear();
		m_currentFrame = 0;
		m_frameTime = 0.0f;
		m_timer = 0.0f;
		m_playing = false;
	}

	unsigned int Animation::getCurrentFrame() const {
		return m_currentFrame;
	}

	void Animation::setCurrentFrame(unsigned int currentFrame) {
		m_currentFrame = currentFrame;
	}

	float Animation::getFrameTime() const {
		return m_frameTime;
	}

	void Animation::setFrameTime(float frameTime) {
		m_frameTime = frameTime;
	}

	bool Animation::isPlaying() const {
		return m_playing;
	}

	// Implement Glyph class functions

	const glm::ivec2& Glyph::getBearing() const {
		return m_bearing;
	}

	void Glyph::setBearing(const glm::ivec2& bearing) {
		m_bearing = bearing;
	}

	unsigned int Glyph::getOffset() const {
		return m_offset;
	}

	void Glyph::setOffset(unsigned int offset) {
		m_offset = offset;
	}

	void Glyph::draw() {
		Sprite::draw();
		m_vertices[9] = 2.0f;
		m_vertices[19] = 2.0f;
		m_vertices[29] = 2.0f;
		m_vertices[39] = 2.0f;
	}

	// Implement Font class functions

	const std::map<char, Glyph>& Font::getGlyphs() const {
		return m_glyphs;
	}

	void Font::addGlyph(char character, const Glyph& glyph) {
		if (m_glyphs.find(character) != m_glyphs.end()) {
			return;
		}
		m_glyphs[character] = glyph;
		if ((float)(glyph.getDimensions().y - glyph.getBearing().y)
			> m_minimumBearing) {
			m_minimumBearing
				= (float)(glyph.getDimensions().y - glyph.getBearing().y);
		}
		if ((float)glyph.getBearing().y > m_maximumBearing) {
			m_maximumBearing = (float)glyph.getBearing().y;
		}
	}

	float Font::getMinimumBearing() const {
		return m_minimumBearing;
	}

	float Font::getMaximumBearing() const {
		return m_maximumBearing;
	}

	void Font::destroy() {
		for (std::map<char, Glyph>::iterator it = m_glyphs.begin();
			it != m_glyphs.end(); it++) {
			it->second.destroy();
		}
		m_glyphs.clear();
		m_minimumBearing = 0.0f;
		m_maximumBearing = 0.0f;
	}
}