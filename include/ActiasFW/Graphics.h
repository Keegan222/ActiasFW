/*
File:		Graphics.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.16@15:03
Purpose:	Contains texture and font loading and OpenGL rendering utilities for
			ActiasFW applications
*/

#ifndef ACTIASFW_GRAPHICS_H
#define ACTIASFW_GRAPHICS_H

#include "Entities.h"

#include <GL/glew.h>
#include <ft2build.h>
#include <freetype/freetype.h>

namespace ActiasFW {
	// A 2D camera with position, velocity, and scale used to generate
	// projection matrices for OpenGL
	class Camera : public Entity {
	public:
		/*
		Initialize this camera's memory and its projection matrix
		*/
		void initialize();
		/*
		Update this camera's position by its velocity and update the projection
		matrix if necessary
		Parameter: float timeStep - The number of frames since the last update
		*/
		void update(float) override;
		/*
		Free this camera's memory
		*/
		void destroy() override;
		/*
		Get the current scale of this camera
		Returns: float - This camera's scale
		*/
		float getScale() const;
		/*
		Set the scale of this camera
		Parameter: float scale - This camera's new scale
		*/
		void setScale(float);
		/*
		Set the current scale velocity of this camera
		Returns: float - This camera's scale velocity
		*/
		float getScaleVelocity() const;
		/*
		Set the scale velocity of this camera
		Parameter: float scaleVelocity - This camera's new scale velocity
		*/
		void setScaleVelocity(float);
		/*
		Get this camera's current projection matrix
		Returns: const glm::mat4& - This camera's projection matrix
		*/
		const glm::mat4& getMatrix() const;

	private:
		// The current scale of this camera
		float m_scale = 0.0f;
		// This camera's current scale velocity
		float m_scaleVelocity = 0.0f;
		// This camera's projection matrix
		glm::mat4 m_matrix = glm::mat4();
		// The position of this camera in the last update
		glm::vec3 m_lastPosition = glm::vec3();
		// The scale of this camera in the last update
		float m_lastScale = 0.0f;
		// The dimensions of the ActiasFW application's window in the last update
		glm::ivec2 m_lastWindowDimensions = glm::ivec2();

		/*
		Update this camera's projection matrix using this camera's position,
		scale, and the window's dimensions
		*/
		void updateMatrix();
	};

	// Wrapper class for an OpenGL GLSL shader program
	class GLSLShader {
	public:
		/*
		Copy another shader's memory into this shader
		Parameter: const GLSLShader& shader - The shader to copy
		*/
		void operator = (const GLSLShader&);
		/*
		Initialize this shader's memory and compile its source code
		Parameter: const std::string& vertexShaderSource - The source code for
		this shader program's vertex shader
		Parameter: const std::string& fragmentShaderSource - The source code for
		this shader program's fragment shader
		Parameter: const std::vector<unsigned int>& vertexAttributeSizes - The
		sizes in floats of the vertex attributes to be drawn by this shader
		Parameter: Camera& camera - The camera whose projection matrix will be
		used for drawing graphics with this shader
		Parameter: const std::string& cameraUniformName - The name of the
		uniform variable to pass the camera's projection matrix to
		Returns: bool - Whether this shader's source code compiled successfully
		*/
		bool initialize(const std::string&, const std::string&,
			const std::vector<unsigned int>&, Camera&, const std::string&);
		/*
		Set OpenGL to use this shader and enable its vertex attributes
		*/
		void begin();
		/*
		Upload an integer to a uniform variable in this shader
		Parameter: const std::string& uniformName - The uniform variable to
		upload to
		Parameter: int value - The value to upload
		Returns: bool - Whether the uniform variable was found
		*/
		bool uploadInt(const std::string&, int);
		/*
		Upload an float to a uniform variable in this shader
		Parameter: const std::string& uniformName - The uniform variable to
		upload to
		Parameter: float value - The value to upload
		Returns: bool - Whether the uniform variable was found
		*/
		bool uploadFloat(const std::string&, float);
		/*
		Upload a 2D vector to a uniform variable in this shader
		Parameter: const std::string& uniformName - The uniform variable to
		upload to
		Parameter: const glm::vec2& value - The value to upload
		Returns: bool - Whether the uniform variable was found
		*/
		bool upload2DVector(const std::string&, const glm::vec2&);
		/*
		Upload a 3D vector to a uniform variable in this shader
		Parameter: const std::string& uniformName - The uniform variable to
		upload to
		Parameter: const glm::vec3& value - The value to upload
		Returns: bool - Whether the uniform variable was found
		*/
		bool upload3DVector(const std::string&, const glm::vec3&);
		/*
		Upload a 4D vector to a uniform variable in this shader
		Parameter: const std::string& uniformName - The uniform variable to
		upload to
		Parameter: const glm::vec4& value - The value to upload
		Returns: bool - Whether the uniform variable was found
		*/
		bool upload4DVector(const std::string&, const glm::vec4&);
		/*
		Upload a 4x4 matrix to a uniform variable in this shader
		Parameter: const std::string& uniformName - The uniform variable to
		upload to
		Parameter: const glm::mat4& value - The value to upload
		Returns: bool - Whether the uniform variable was found
		*/
		bool upload4x4Matrix(const std::string&, const glm::mat4&);
		/*
		Set OpenGL to stop using this shader and disable its vertex attributes
		*/
		void end();
		/*
		Delete this shader from OpenGL and free its memory
		*/
		void destroy();
		/*
		Get this shader program's vertex shader source code
		Returns: const std::string& - This shader's vertex shader source code
		*/
		const std::string& getVertexShaderSource() const;
		/*
		Set this shader program's vertex shader source code
		Parameter: const std::string& vertexShaderSource - This shader's new
		vertex shader source code
		*/
		void setVertexShaderSource(const std::string&);
		/*
		Get the shader program's fragment shader source code
		Returns: const std::string& - This shader's fragment shader source code
		*/
		const std::string& getFragmentShaderSource() const;
		/*
		Set this shader program's fragment shader source code
		Parameter: const std::string& fragmentShaderSource - This shader's new
		fragment shader source code
		*/
		void setFragmentShaderSource(const std::string&);
		/*
		Get the OpenGL ID of this shader program
		Returns: GLuint - This shader's ID
		*/
		GLuint getShaderID() const;
		/*
		Get the number of floats in each vertex attribute drawn by this shader
		Returns: std::vector<unsigned int> - This shader's vertex attribute
		sizes
		*/
		std::vector<unsigned int> getVertexAttributeSizes() const;
		/*
		Set the number of floats in each vertex attribute to be drawn by this
		shader
		Parameter: const std::string<unsigned int>& vertexAttributeSizes - This
		shader's new vertex attribute sizes
		*/
		void setVertexAttributeSizes(const std::vector<unsigned int>&);
		/*
		Get the camera whose projection matrix is used in this shader
		Returns: Camera& - This shader's camera
		*/
		Camera& getCamera();
		/*
		Set the camera whose projection matrix is to be used in this shader
		Parameter: Camera& camera - This shader's new camera
		*/
		void setCamera(Camera&);
		/*
		Get the name of the uniform variable to upload this shader's camera
		matrix to
		Returns: const std::string& - The camera matrix uniform variable name
		*/
		const std::string& getCameraUniformName() const;
		/*
		Set the name of the uniform variable to upload this shader's camera
		matrix to
		Parameter: const std::string& - The new camera matrix uniform variable
		name
		*/
		void setCameraUniformName(const std::string&);

	private:
		// This shader program's vertex shader source code
		std::string m_vertexShaderSource = "";
		// This shader program's fragment shader source code
		std::string m_fragmentShaderSource = "";
		// This shader's OpenGL ID
		GLuint m_shaderID = 0;
		// This shader's vertex attribute sizes (in floats) and offsets (in
		// bytes)
		std::vector<std::pair<unsigned int, unsigned int>> m_vertexAttributes;
		// This shader's vertex attribute strides
		unsigned int m_vertexAttributeStride = 0;
		// This shader's camera
		Camera* m_camera = 0;
		// This shader's camera matrix uniform name
		std::string m_cameraUniformName = "";

		/*
		Compile this shader program's vertex and fragment shader source code and
		populate its OpenGL ID
		Returns: bool - Whether this shader compiled successfully
		*/
		bool compile();
		/*
		Get the location of a uniform variable by name in this shader
		Parameter: const std::string& uniformName - The uniform to locate
		Parameter: GLint& location - The location to populate
		Returns: bool - Whether the uniform was found
		*/
		bool getUniformLocation(const std::string&, GLint& location);
	};

	// The OpenGL graphics system for ActiasFW applications
	class GraphicsManager {
	public:
		/*
		Initialize the GLEW and FreeType libraries and this graphics manager's
		memory
		Parameter: const glm::vec3& backgroundColor - The color to clear the
		application's window to
		Returns: bool - Whether the graphics system could be initialized
		*/
		bool initialize(const glm::vec3&);
		/*
		Update this graphics manager's default camera
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		void update(float);
		/*
		Clear the window and start a new frame
		*/
		void begin();
		/*
		Submit a sprite to be drawn this frame with a shader
		Parameter: T& sprite - The sprite to draw
		Parameter: GLSLShader& shader - The shader to use when drawing
		*/
		template <typename T>
		void submit(T& sprite, GLSLShader& shader) {
			sprite.draw();
			if (sprite.getVertices().empty()) {
				return;
			}
			m_sprites.push_back(
				std::pair<GLSLShader*, Sprite*>(&shader, (Sprite*)&sprite));
		}
		/*
		Submit a sprite to be drawn this frame with the default shader
		Parameter: T& sprite - The sprite to draw
		*/
		template <typename T>
		void submit(T& sprite) {
			submit(sprite, m_defaultShader);
		}
		/*
		Submit a set of sprites to be drawn this frame with a shader
		Parameter: std::vector<T>& sprites - The sprites to draw
		Parameter: GLSLShader& shader - The shader to use when drawing
		*/
		template <typename T>
		void submit(std::vector<T>& sprites, GLSLShader& shader) {
			for (T& sprite : sprites) {
				submit(sprite, shader);
			}
		}
		/*
		Submit a set of sprites to be drawn this frame with the default shader
		Parameter: std::vector<T>& sprites - The sprites to draw
		*/
		template <typename T>
		void submit(std::vector<T>& sprites) {
			submit(sprites, m_defaultShader);
		}
		/*
		Submit text to be drawn this frame with a shader
		Parameter: const std::string& text - The text to draw
		Parameter: const glm::vec3& posiion - The position and depth to draw the
		text at
		Parameter: const glm::vec4& box - The bounding box to draw the text
		inside of (x, y, width, height)
		Parameter: float scale - The scale to draw the text at
		Parameter: const glm::vec4& color - The color to draw the text in (RGBA)
		Parameter: const std::string& fontFileName - The file name of the font
		to draw the text in
		Parameter: Alignment horizontalAlignment - The horizontal alignment to
		draw the text in inside its box
		Parameter: Alignment verticalAlignment - The vertical alignment to draw
		the text in inside its box
		Parameter: GLSLShader& shader - The shader to use when drawing
		*/
		void submit(const std::string&, const glm::vec3&, const glm::vec4&,
			float, const glm::vec4&, const std::string&, Alignment, Alignment,
			GLSLShader&);
		/*
		Submit text to be drawn this frame with the default shader
		Parameter: const std::string& text - The text to draw
		Parameter: const glm::vec3& posiion - The position and depth to draw the
		text at
		Parameter: const glm::vec4& box - The bounding box to draw the text
		inside of (x, y, width, height)
		Parameter: float scale - The scale to draw the text at
		Parameter: const glm::vec4& color - The color to draw the text in (RGBA)
		Parameter: const std::string& fontFileName - The file name of the font
		to draw the text in
		Parameter: Alignment horizontalAlignment - The horizontal alignment to
		draw the text in inside its box
		Parameter: Alignment verticalAlignment - The vertical alignment to draw
		the text in inside its box
		*/
		void submit(const std::string&, const glm::vec3&, const glm::vec4&,
			float, const glm::vec4&, const std::string&, Alignment, Alignment);
		/*
		Draw all the sprites and text submitted this frame
		*/
		void end();
		/*
		Terminate the GLEW and FreeType libraries and free this graphics
		manager's memory
		Returns: bool - Whether this graphics manager was destroyed successfully
		*/
		bool destroy();
		/*
		Get the default camera used when drawing sprites with this graphics
		manager
		Returns: Camera& - This graphics manager's default camera
		*/
		Camera& getDefaultCamera();
		/*
		Get the default shader used when drawing sprites with this graphics
		manager
		Returns: GLSLShader& - This graphics manager's default shader
		*/
		GLSLShader& getDefaultShader();

	private:
		// Allow the UITextBox class access to private memory
		friend class UITextBox;
		// Allow the UIPasswordBox class access to private memory
		friend class UIPasswordBox;
		// Allow the UITextArea class access to private memory
		friend class UITextArea;
		// Whether this graphics manager has been initialized
		bool m_initialized = false;
		// The ID of the OpenGL vertex array object used to draw graphics
		GLuint m_VAOID = 0;
		// The ID of the OpenGL vertex buffer object used to draw graphics
		GLuint m_VBOID = 0;
		// The ID of the OpenGL index buffer object used to draw graphics
		GLuint m_IBOID = 0;
		// The set of textures loaded by OpenGL
		std::map<std::string, GLuint> m_textures;
		// The set of sprites submitted to be drawn in the current frame
		std::vector<std::pair<GLSLShader*, Sprite*>> m_sprites;
		// The instance of the FreeType library used by this graphics manager
		// to draw text
		FT_Library m_freeType = 0;
		// The set of fonts loaded by FreeType
		std::map<std::string, Font> m_fonts;
		// The set of text characters submitted to be drawn in the current frame
		std::map<GLSLShader*, std::vector<Glyph>> m_glyphs;
		// The default camera used by this graphics manager
		Camera m_defaultCamera;
		// The default shader used by this graphics manager
		GLSLShader m_defaultShader;

		/*
		Get an OpenGL texture ID by its file name or attempt to decode it from
		disk and load it into OpenGL
		Parameter: const std::string& fileName - The file name of the texture to
		load
		Returns: const GLuint - The OpenGL texture ID or 0 if the texture could
		not be loaded
		*/
		const GLuint getTexture(const std::string&);
		/*
		Get a font from memory or attempt to load it from disk with the
		FreeType library
		Parameter: const std::string& fileName - The file name of the font to
		load
		Returns: const Font* - A pointer to the font or 0 if the font could not
		be loaded
		*/
		const Font* getFont(const std::string&);
		/*
		Draw a set of indices of a set of vertices with OpenGL
		Parameter: const std::vector<unsigned int>& indices - The indices of the
		vertex data to draw
		Parameter: const std::vector<float>& vertices - The vertex data to draw
		*/
		void draw(const std::vector<unsigned int>&, const std::vector<float>&);
	};
}

#endif