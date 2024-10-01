/*
File:		Entities.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.16@15:01
Purpose:	Contains classes representing abstract, static, and animated
			entities which can be drawn by the graphics module
*/

#ifndef ACTIASFW_ENTITIES_H
#define ACTIASFW_ENTITIES_H

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace ActiasFW {
	// An abstract entity with a 2D position, depth, and 2D velocity
	class Entity {
	public:
		/*
		Initialize this entity's memory
		Parameter: const glm::vec3& position - The position and depth of this
		entity
		Parameter: const glm::vec2& velocity - The velocity of this entity
		*/
		void initialize(const glm::vec3&, const glm::vec2&);
		/*
		Update this entity's position by its velocity
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float);
		/*
		Free this entity's memory
		*/
		virtual void destroy();
		/*
		Get this entity's position and depth
		Returns: const glm::vec3& - This entity's position and depth
		*/
		virtual const glm::vec3& getPosition() const;
		/*
		Set this entity's position and depth
		Parameter: const glm::vec3& position - The new position and depth of
		this entity
		*/
		virtual void setPosition(const glm::vec3&);
		/*
		Get this entity's velocity
		Returns: const glm::vec2& - This entity's velocity
		*/
		virtual const glm::vec2& getVelocity() const;
		/*
		Set this entity's velocity
		Parameter: const glm::vec2& velocity - The new velocity of this entity
		*/
		virtual void setVelocity(const glm::vec2&);

	protected:
		// The position and depth of this entity
		glm::vec3 m_position = glm::vec3();
		// The velocity of this entity
		glm::vec2 m_velocity = glm::vec2();
	};

	// An abstract 2D sprite with a position, velocity, depth, and texture
	class Sprite : public Entity {
	public:
		/*
		Initialize this sprite's memory
		Parameter: const glm::vec3& position - The position and depth of this
		entity
		Parameter: const glm::vec2& dimensions - The width and height of this
		entity
		Parameter: const std::string& textureFileName - The file name of this
		entity's texture
		*/
		void initialize(const glm::vec3&, const glm::vec2&, const std::string&);
		/*
		Update this sprite's position by its velocity and its rotation
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Test whether another sprite intersects with this sprite
		Parameter: const Sprite& sprite - The sprite to test
		Returns: bool - Whether the given sprite intersects with this sprite
		*/
		virtual bool intersects(const Sprite&);
		/*
		Free this sprite's memory
		*/
		virtual void destroy() override;
		/*
		Get this sprite's dimensions
		Returns: const glm::vec2& - This sprite's width and height
		*/
		virtual const glm::vec2& getDimensions() const;
		/*
		Set this sprite's dimensions
		Parameter: const glm::vec2& dimensions - The new width and height of
		this sprite
		*/
		virtual void setDimensions(const glm::vec2&);
		/*
		Get the current rotation angle of this sprite
		Returns: float - The rotation in degrees of this sprite
		*/
		virtual float getRotation() const;
		/*
		Set the rotation angle of this sprite
		Parameter: float rotation - The new rotation in degrees for this sprite
		*/
		virtual void setRotation(float);
		/*
		Get the current rotation velocity of this sprite
		Returns: float - The rotation velocity of this sprite
		*/
		virtual float getRotationVelocity() const;
		/*
		Set the rotation velocity of this sprite
		Parameter: float - The new rotation velocity for this sprite
		*/
		virtual void setRotationVelocity(float);
		/*
		Get the file name of this sprite's texture
		Returns: const std::string& - This file name of this sprite's texture
		*/
		virtual const std::string& getTextureFileName() const;
		/*
		Set the file name of this sprite's texture
		Parameter: const std::string& textureFileName - The file name of this
		sprite's new texture
		*/
		virtual void setTextureFileName(const std::string&);
		/*
		Get the current texture coordinates of this sprite
		Returns: const glm::vec4& - The texture coordinates of this sprite
		(x, y, width, height)
		*/
		virtual const glm::vec4& getTextureBox() const;
		/*
		Set the texture coordinates of this sprite
		Parameter: const glm::vec4& textureBox - The new texture coordinates for
		this sprite (x, y, width, height)
		*/
		virtual void setTextureBox(const glm::vec4&);
		/*
		Get the current color of this sprite
		Returns: const glm::vec4& - The color of this sprite (RGBA)
		*/
		virtual const glm::vec4& getColor() const;
		/*
		Set the color of this sprite
		Parameter: const glm::vec4& color - The new color for this sprite (RGBA)
		*/
		virtual void setColor(const glm::vec4&);
		/*
		Test whether this sprite's image is reflected horizontally
		Returns: bool - Whether this sprite is reflected horizontally
		*/
		virtual bool isReflectedHorizontally() const;
		/*
		Set whether this sprite's image is reflected horizontally
		Parameter: bool reflectedHorizontally - Whether this sprite should be
		reflected horizontally
		*/
		virtual void setReflectedHorizontally(bool);
		/*
		Test whether this sprite's image is reflected vertically
		Returns: bool - Whether this sprite is reflected vertically
		*/
		virtual bool isReflectedVertically() const;
		/*
		Set whether this sprite's image is reflected vertically
		Parameter: bool reflectedVertically - Whether this sprite should be
		reflected vertically
		*/
		virtual void setReflectedVertically(bool);
		/*
		Get the vertex indices for this sprite to be drawn with by OpenGL
		Returns: const std::vector<unsigned int>& - This sprite's indices
		*/
		virtual const std::vector<unsigned int>& getIndices() const;
		/*
		Get the vertex data for this sprite to be drawn with by OpenGL
		Returns: const std::vector<float>& - This sprite's vertex data
		*/
		virtual const std::vector<float>& getVertices() const;

	protected:
		friend class GraphicsManager;
		// The width and height of this sprite
		glm::vec2 m_dimensions = glm::vec2();
		// The current rotation angle of this sprite in degrees
		float m_rotation = 0.0f;
		// The rotation velocity of this sprite
		float m_rotationVelocity = 0.0f;
		// The file name of this sprite's texture
		std::string m_textureFileName = "";
		// The texture coordinates for this sprite
		glm::vec4 m_textureBox = glm::vec4();
		// The RGBA color of this sprite
		glm::vec4 m_color = glm::vec4();
		// Whether this sprite's image is reflected horizontally
		bool m_reflectedHorizontally = false;
		// Whether this sprite's image is reflected vertically
		bool m_reflectedVertically = false;
		// This sprite's indices to be drawn by OpenGL
		std::vector<unsigned int> m_indices;
		// This sprite's vertices to be drawn by OpenGL
		std::vector<float> m_vertices;

		/*
		Populate this sprite's indices and vertices by its position, dimensions,
		texture, rotation, reflections, and color
		*/
		virtual void draw();

	private:
		/*
		Rotate a point about this sprite's center
		Parameter: const glm::vec3& p - The point to rotate
		Returns: glm::vec3 - The rotated point
		*/
		glm::vec3 rotate(const glm::vec3&) const;
		/*
		Swap two indices of this sprite's vertex data
		Parameter: size_t a - The first index to swap
		Parameter: size_t b - The second index to swap
		*/
		void transposeVertices(size_t, size_t);
	};

	// An abstract animated 2D sprite
	class Animation : public Sprite {
	public:
		/*
		Initialize this animation's memory
		Parameter: const glm::vec3& position - The position and depth of this
		animation
		Parameter: const glm::vec2& dimensions - The width and height of this
		animation
		Parameter: const std::string& textureFileName - The file name of this
		animation's texture
		Parameter: const glm::ivec2& frameDimensions - The number of frames of
		animation in the texture horizontally and vertically
		Parameter: float frameTime - The number of updates to play each frame
		of this animation for
		*/
		void initialize(const glm::vec3&, const glm::vec2&, const std::string&,
			const glm::ivec2&, float);
		/*
		Update this sprite's position by its velocity, its rotation, and the
		animation frame
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Start or resume playing this animation
		*/
		virtual void play();
		/*
		Stop playing this animation
		*/
		virtual void pause();
		/*
		Stop playing this animation and set it to the starting frame
		*/
		virtual void stop();
		/*
		Free this animation's memory
		*/
		virtual void destroy() override;
		/*
		Get the current frame of this animation
		Returns: unsigned int - The current frame of this animation
		*/
		virtual unsigned int getCurrentFrame() const;
		/*
		Set the current frame of this animation
		Parameter: unsigned int currentFrame - The new current frame of this
		animation
		*/
		virtual void setCurrentFrame(unsigned int);
		/*
		Get the number of frames to play each frame of this animation for
		Returns: float - This animation's frame time
		*/
		virtual float getFrameTime() const;
		/*
		Set the number of frames to play each frame of this animation for
		Parameter: float frameTime - This animation's new frame time
		*/
		virtual void setFrameTime(float);
		/*
		Test whether this animation is currently playing
		Returns: bool - Whether this animation of playing
		*/
		virtual bool isPlaying() const;

	protected:
		// The texture coordinates of the frames of this animation
		std::vector<glm::vec2> m_frameCoordinates;
		// The current frame of this animation
		unsigned int m_currentFrame = 0;
		// The number of frames to play each frame of this animation for
		float m_frameTime = 0.0f;
		// Timer for tracking frame times
		float m_timer = 0.0f;
		// Whether this animation is currently playing
		bool m_playing = false;
	};

	// Enumeration of possiblealignments for text rendered in ActiasFW
	// applications
	enum Alignment {
		ALIGN_LEFT,
		ALIGN_RIGHT,
		ALIGN_BOTTOM,
		ALIGN_TOP,
		ALIGN_CENTER,
		NO_ALIGNMENT,
	};

	// A single text character in a font
	class Glyph : public Sprite {
	public:
		/*
		Get this glyph's horizontal and vertical bearings
		Returns: const glm::ivec2& - This glyph's bearing
		*/
		const glm::ivec2& getBearing() const;
		/*
		Set this glyph's horizontal and vertical bearings
		Parameter: const glm::ivec2& bearing - The new bearing for this glyph
		*/
		void setBearing(const glm::ivec2&);
		/*
		Get the horizontal offset of this glyph
		Returns: unsigned int - This glyph's offset
		*/
		unsigned int getOffset() const;
		/*
		Set the horizontal offset of this glyph
		Parameter: unsigned int offset - The new offset for this glyph
		*/
		void setOffset(unsigned int);

	private:
		friend class GraphicsManager;
		// This glyph's horizontal and vertical bearings
		glm::ivec2 m_bearing = glm::ivec2();
		// This glyph's horizontal offset
		unsigned int m_offset = 0;

		/*
		Populate this glyph's indices and vertices for OpenGL to draw
		*/
		void draw() override;
	};

	// A set of glyphs associated with text characters
	class Font {
	public:
		/*
		Get the set of glyphs in this font
		Returns: const std::map<char, Glyph>& - The glyphs in this font
		*/
		const std::map<char, Glyph>& getGlyphs() const;
		/*
		Add a glyph to this font by its text character
		Parameter: char character - The text character to add
		Parameter: const Glyph& glyph - The glyph to add
		*/
		void addGlyph(char, const Glyph&);
		/*
		Get the minimum vertical bearing of the glyphs in this font
		Returns: float - This font's minimum bearing
		*/
		float getMinimumBearing() const;
		/*
		Get the maximum vertical bearing of the glyphs in this font
		Returns: float - This font's maximum bearing
		*/
		float getMaximumBearing() const;
		/*
		Free this font's memory and that of its glyphs
		*/
		void destroy();

	private:
		// The set of glyphs in this font
		std::map<char, Glyph> m_glyphs;
		// The minimum vertical bearing of the glyphs in this font
		float m_minimumBearing = 0.0f;
		// The maximum vertical bearing of the glyphs in this font
		float m_maximumBearing = 0.0f;
	};
}

#endif