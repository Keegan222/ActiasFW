/*
File:		Graphics.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.16@15:03
Purpose:	Defines variables and implements functions found in Graphics.h
*/

#include "ActiasFW.h"

#include <glm/gtc/matrix_transform.hpp>
#include <lodepng.h>

#include <algorithm>

namespace ActiasFW {
	// Implement Camera class functions

	void Camera::initialize() {
		Entity::initialize(glm::vec3(), glm::vec2());
		setScale(1.0f);
		setScaleVelocity(0.0f);
		m_lastPosition = glm::vec3();
		m_lastScale = 1.0f;
		m_lastWindowDimensions = Application::Window.getDimensions();
		updateMatrix();
	}

	void Camera::update(float timeStep) {
		Entity::update(timeStep);
		m_scale += m_scaleVelocity * m_scale * timeStep;
		if (m_scale < 0.0f) {
			m_scale = 0.0f;
		}
		if (m_position != m_lastPosition || m_scale != m_lastScale
			|| m_lastWindowDimensions != Application::Window.getDimensions()) {
			updateMatrix();
		};
	}

	void Camera::destroy() {
		Entity::destroy();
		m_scale = 0.0f;
		m_scaleVelocity = 0.0f;
		m_matrix = glm::mat4();
		m_lastPosition = glm::vec3();
		m_lastScale = 0.0f;
		m_lastWindowDimensions = glm::ivec2();
	}

	float Camera::getScale() const {
		return m_scale;
	}

	void Camera::setScale(float scale) {
		m_scale = scale;
	}

	float Camera::getScaleVelocity() const {
		return m_scaleVelocity;
	}

	void Camera::setScaleVelocity(float scaleVelocity) {
		m_scaleVelocity = scaleVelocity;
	}

	const glm::mat4& Camera::getMatrix() const {
		return m_matrix;
	}

	void Camera::updateMatrix() {
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		glm::vec2 ws(wd.x / m_lastWindowDimensions.x,
			wd.y / m_lastWindowDimensions.y);
		m_position.x *= ws.x;
		m_position.y *= ws.y;
		m_matrix = glm::ortho(-wd.x / 2.0f, wd.x / 2.0f,
			-wd.y / 2.0f, wd.y / 2.0f);
		m_matrix = glm::translate(m_matrix,
			glm::vec3(-m_position.x, -m_position.y, 0.0f));
		m_matrix = glm::scale(glm::mat4(1.0f),
			glm::vec3(m_scale, m_scale, 0.0f)) * m_matrix;
		m_lastPosition = m_position;
		m_lastScale = m_scale;
		m_lastWindowDimensions = Application::Window.getDimensions();
	}

	// Implement GLSLShader class functions

	void GLSLShader::operator = (const GLSLShader& shader) {
		setVertexShaderSource(shader.getVertexShaderSource());
		setFragmentShaderSource(shader.getFragmentShaderSource());
		setVertexAttributeSizes(shader.getVertexAttributeSizes());
		setCamera(*shader.m_camera);
		setCameraUniformName(shader.getCameraUniformName());
		compile();
	}

	bool GLSLShader::initialize(const std::string& vertexShaderSource,
		const std::string& fragmentShaderSource,
		const std::vector<unsigned int>& vertexAttributeSizes, Camera& camera,
		const std::string& cameraUniformName) {
		setVertexShaderSource(vertexShaderSource);
		setFragmentShaderSource(fragmentShaderSource);
		setVertexAttributeSizes(vertexAttributeSizes);
		setCamera(camera);
		setCameraUniformName(cameraUniformName);
		return compile();
	}

	void GLSLShader::begin() {
		glUseProgram(m_shaderID);
		for (unsigned int a = 0; a < m_vertexAttributes.size(); a++) {
			glVertexAttribPointer(a, m_vertexAttributes.at(a).first, GL_FLOAT,
				GL_FALSE, m_vertexAttributeStride,
				(void*)m_vertexAttributes.at(a).second);
			glEnableVertexAttribArray(a);
		}
		if (!m_cameraUniformName.empty()) {
			upload4x4Matrix(m_cameraUniformName, m_camera->getMatrix());
		}
	}

	bool GLSLShader::uploadInt(const std::string& uniformName, int value) {
		GLint location;
		if (!getUniformLocation(uniformName, location)) {
			return false;
		}
		glUniform1i(location, (GLint)value);
		return true;
	}

	bool GLSLShader::uploadFloat(const std::string& uniformName, float value) {
		GLint location;
		if (!getUniformLocation(uniformName, location)) {
			return false;
		}
		glUniform1f(location, value);
		return true;
	}

	bool GLSLShader::upload2DVector(const std::string& uniformName,
		const glm::vec2& value) {
		GLint location;
		if (!getUniformLocation(uniformName, location)) {
			return false;
		}
		glUniform2f(location, value.x, value.y);
		return true;
	}

	bool GLSLShader::upload3DVector(const std::string& uniformName,
		const glm::vec3& value) {
		GLint location;
		if (!getUniformLocation(uniformName, location)) {
			return false;
		}
		glUniform3f(location, value.x, value.y, value.z);
		return true;
	}

	bool GLSLShader::upload4DVector(const std::string& uniformName,
		const glm::vec4& value) {
		GLint location;
		if (!getUniformLocation(uniformName, location)) {
			return false;
		}
		glUniform4f(location, value.x, value.y, value.z, value.w);
		return true;
	}

	bool GLSLShader::upload4x4Matrix(const std::string& uniformName,
		const glm::mat4& value) {
		GLint location;
		if (!getUniformLocation(uniformName, location)) {
			return false;
		}
		glUniformMatrix4fv(location, 1, GL_FALSE, &(value[0][0]));
		return true;
	}

	void GLSLShader::end() {
		for (unsigned int a = 0; a < m_vertexAttributes.size(); a++) {
			glDisableVertexAttribArray(a);
		}
		glUseProgram(0);
	}

	void GLSLShader::destroy() {
		end();
		m_vertexShaderSource = "";
		m_fragmentShaderSource = "";
		glDeleteProgram(m_shaderID);
		m_shaderID = 0;
		m_vertexAttributes.clear();
		m_vertexAttributeStride = 0;
		m_camera = 0;
		m_cameraUniformName = "";
	}

	const std::string& GLSLShader::getVertexShaderSource() const {
		return m_vertexShaderSource;
	}

	void GLSLShader::setVertexShaderSource(
		const std::string& vertexShaderSource) {
		m_vertexShaderSource = vertexShaderSource;
	}

	const std::string& GLSLShader::getFragmentShaderSource() const {
		return m_fragmentShaderSource;
	}

	void GLSLShader::setFragmentShaderSource(
		const std::string& fragmentShaderSource) {
		m_fragmentShaderSource = fragmentShaderSource;
	}

	GLuint GLSLShader::getShaderID() const {
		return m_shaderID;
	}

	std::vector<unsigned int> GLSLShader::getVertexAttributeSizes() const {
		std::vector<unsigned int> vertexAttributeSizes;
		for (const std::pair<unsigned int, unsigned int>& attribute
			: m_vertexAttributes) {
			vertexAttributeSizes.push_back(attribute.first);
		}
		return vertexAttributeSizes;
	}

	void GLSLShader::setVertexAttributeSizes(
		const std::vector<unsigned int>& vertexAttributeSizes) {
		m_vertexAttributes.resize(vertexAttributeSizes.size());
		m_vertexAttributeStride = 0;
		for (unsigned int a = 0; a < vertexAttributeSizes.size(); a++) {
			m_vertexAttributes[a].first = vertexAttributeSizes.at(a);
			m_vertexAttributes[a].second
				= m_vertexAttributeStride * sizeof(float);
			m_vertexAttributeStride += vertexAttributeSizes.at(a);
		}
		m_vertexAttributeStride *= sizeof(float);
	}

	Camera& GLSLShader::getCamera() {
		return *m_camera;
	}

	void GLSLShader::setCamera(Camera& camera) {
		m_camera = &camera;
	}

	const std::string& GLSLShader::getCameraUniformName() const {
		return m_cameraUniformName;
	}

	void GLSLShader::setCameraUniformName(
		const std::string& cameraUniformName) {
		m_cameraUniformName = cameraUniformName;
	}

	bool GLSLShader::compile() {
		const char* vSource = m_vertexShaderSource.c_str();
		GLuint vID = glCreateShader(GL_VERTEX_SHADER);
		ACTIASFW_LOG(GLSLShader, Info, "Compiling vertex shader ", vID);
		glShaderSource(vID, 1, &vSource, 0);
		glCompileShader(vID);
		int vSuccess;
		glGetShaderiv(vID, GL_COMPILE_STATUS, &vSuccess);
		if (!vSuccess) {
			char vBuffer[1024];
			glGetShaderInfoLog(vID, 1024, 0, vBuffer);
			ACTIASFW_LOG(GLSLShader, Warning, "Failed to compile vertex ",
				"shader ", vID, "\n", vBuffer);
			return false;
		}
		const char* fSource = m_fragmentShaderSource.c_str();
		GLuint fID = glCreateShader(GL_FRAGMENT_SHADER);
		ACTIASFW_LOG(GLSLShader, Info, "Compiling fragment shader ", fID);
		glShaderSource(fID, 1, &fSource, 0);
		glCompileShader(fID);
		int fSuccess;
		glGetShaderiv(fID, GL_COMPILE_STATUS, &fSuccess);
		if (!fSuccess) {
			char fBuffer[1024];
			glGetShaderInfoLog(fID, 1024, 0, fBuffer);
			ACTIASFW_LOG(GLSLShader, Warning, "Failed to compile fragment ",
				"shader ", fID, "\n", fBuffer);
			return false;
		}
		m_shaderID = glCreateProgram();
		glAttachShader(m_shaderID, vID);
		glAttachShader(m_shaderID, fID);
		ACTIASFW_LOG(GLSLShader, Info, "Linking shader program ", m_shaderID);
		glLinkProgram(m_shaderID);
		int pSuccess;
		glGetProgramiv(m_shaderID, GL_LINK_STATUS, &pSuccess);
		if (!pSuccess) {
			char pBuffer[1024];
			glGetProgramInfoLog(m_shaderID, 1024, 0, pBuffer);
			ACTIASFW_LOG(GLSLShader, Warning, "Failed to link shader program ",
				m_shaderID);
			return false;
		}
		glDeleteShader(vID);
		glDeleteShader(fID);
		return true;
	}

	bool GLSLShader::getUniformLocation(const std::string& uniformName,
		GLint& location) {
		location = glGetUniformLocation(m_shaderID, uniformName.c_str());
		return location != GL_INVALID_VALUE && location != GL_INVALID_OPERATION;
	}

	// Implement GraphicsManager class functions

	bool GraphicsManager::initialize(const glm::vec3& backgroundColor) {
		ACTIASFW_LOG(Graphics, Info, "Initializing graphics manager");
		if (m_initialized) {
			ACTIASFW_LOG(Graphics, Error, "Graphics manager already ",
				"initialized");
			return false;
		}
		ACTIASFW_LOG(Graphics, Info, "Initializing GLEW library");
		if (glewInit() != GLEW_OK) {
			ACTIASFW_LOG(Graphics, Error, "Failed to initialize GLEW");
			return false;
		}
		const GLubyte* glVersion = glGetString(GL_VERSION);
		ACTIASFW_LOG(Graphics, Info, glVersion);
		ACTIASFW_LOG(Graphics, Info, "Initializing FreeType library");
		if (FT_Init_FreeType(&m_freeType) != FT_Err_Ok) {
			ACTIASFW_LOG(Graphics, Info, "Failed to initialize FreeType");
			return false;
		}
		ACTIASFW_LOG(Graphics, Info, "Initializing OpenGL parameters");
		glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b,
			1.0f);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glPixelStorei(GL_UNPACK_ALIGNMENT, GL_TRUE);
		glfwSwapInterval(1);
		glGenVertexArrays(1, &m_VAOID);
		glBindVertexArray(m_VAOID);
		glGenBuffers(1, &m_VBOID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
		glGenBuffers(1, &m_IBOID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
		m_defaultCamera.initialize();
		m_defaultShader.initialize(
			"#version 330 core\n"
			"layout (location = 0) in vec3 vertexPosition;\n"
			"layout (location = 1) in vec2 vertexTexturePosition;\n"
			"layout (location = 2) in vec4 vertexColor;\n"
			"layout (location = 3) in float vertexMode;\n"
			"uniform mat4 cameraMatrix;\n"
			"out vec2 fragmentTexturePosition;\n"
			"out vec4 fragmentColor;\n"
			"out float fragmentMode;\n"
			"void main() {\n"
			"	gl_Position = cameraMatrix * vec4(vertexPosition, 1.0);\n"
			"	fragmentTexturePosition = vertexTexturePosition;\n"
			"	fragmentColor = vertexColor;\n"
			"	fragmentMode = vertexMode;\n"
			"}\n",
			"#version 330 core\n"
			"in vec2 fragmentTexturePosition;\n"
			"in vec4 fragmentColor;\n"
			"in float fragmentMode;\n"
			"uniform sampler2D textureSampler;\n"
			"void main() {\n"
			"	if (fragmentMode >= 2.0) {\n"
			"		vec4 textureColor = vec4(1.0, 1.0, 1.0,\n"
			"			texture(textureSampler, fragmentTexturePosition).r);\n"
			"		gl_FragColor = fragmentColor * textureColor;\n"
			"	} else {\n"
			"		gl_FragColor = (1.0 - fragmentMode)"
			"			* texture(textureSampler, fragmentTexturePosition)"
			"			+ fragmentMode * fragmentColor;\n"
			"	}\n"
			"}\n",
			{ 3, 2, 4, 1 },
			m_defaultCamera, "cameraMatrix");
		m_initialized = true;
		return true;
	}

	void GraphicsManager::update(float timeStep) {
		m_defaultCamera.update(timeStep);
	}

	void GraphicsManager::begin() {
		glClear(GL_COLOR_BUFFER_BIT);
		m_sprites.clear();
		m_glyphs.clear();
	}

	void GraphicsManager::submit(const std::string& text,
		const glm::vec3& position, const glm::vec4& box, float scale,
		const glm::vec4& color, const std::string& fontFileName,
		Alignment horizontalAlignment, Alignment verticalAlignment,
		GLSLShader& shader) {
		if (text.empty()) {
			return;
		}
		const Font* font = getFont(fontFileName);
		if (font == 0) {
			return;
		}
		if (font->getGlyphs().empty()) {
			return;
		}
		if (m_glyphs.find(&shader) == m_glyphs.end()) {
			m_glyphs[&shader] = std::vector<Glyph>(0);
		}
		float labelWidth = 0.0f;
		for (char c : text) {
			labelWidth += font->getGlyphs().at(c).getOffset();
		}
		labelWidth *= scale;
		float x = 0.0f;
		if (horizontalAlignment == ALIGN_LEFT) {
			x = box.x;
		}
		else if (horizontalAlignment == ALIGN_RIGHT) {
			x = (box.x + box.z) - labelWidth;
		}
		else if (horizontalAlignment == ALIGN_CENTER) {
			float m = box.x + (box.z / 2.0f);
			x = m - (labelWidth / 2.0f);
		}
		else {
			x = position.x;
		}
		x -= font->getGlyphs().at(text.at(0)).getBearing().x * scale;
		float labelHeight
			= (font->getMinimumBearing() + font->getMaximumBearing()) * scale;
		float y = 0.0f;
		if (verticalAlignment == ALIGN_BOTTOM) {
			y = box.y + (font->getMinimumBearing() * scale);
		}
		else if (verticalAlignment == ALIGN_TOP) {
			y = (box.y + box.w) - labelHeight;
		}
		else if (verticalAlignment == ALIGN_CENTER) {
			float m = box.y + (box.w / 2.0f);
			y = m - (labelHeight / 2.0f);
		}
		else {
			y = position.y;
		}
		for (unsigned int i = 0; i < text.length(); i++) {
			char c = text.at(i);
			Glyph g = font->getGlyphs().at(c);
			g.initialize(glm::vec3(x + (g.getBearing().x * scale),
				y - ((g.getDimensions().y - g.getBearing().y) * scale),
				position.z), g.getDimensions() * scale, g.getTextureFileName());
			g.setColor(color);
			x += (float)g.getOffset() * scale;
			if (g.getPosition().x < box.x
				|| g.getPosition().x + g.getDimensions().x > box.x + box.z) {
				continue;
			}
			if (g.getPosition().y < box.y
				|| g.getPosition().y + g.getDimensions().y > box.y + box.w) {
				continue;
			}
			m_glyphs[&shader].push_back(g);
		}
	}

	void GraphicsManager::submit(const std::string& text,
		const glm::vec3& position, const glm::vec4& box, float scale,
		const glm::vec4& color, const std::string& fontFileName,
		Alignment horizontalAlignment, Alignment verticalAlignment) {
		submit(text, position, box, scale, color, fontFileName,
			horizontalAlignment, verticalAlignment, m_defaultShader);
	}

	bool CompareSpriteDepth(const std::pair<GLSLShader*, Sprite*>& a,
		const std::pair<GLSLShader*, Sprite*>& b) {
		if (a.second == 0 || b.second == 0) {
			return false;
		}
		return a.second->getPosition().z < b.second->getPosition().z;
	}

	void GraphicsManager::end() {
		if (!m_glyphs.empty()) {
			for (std::pair<GLSLShader*, std::vector<Glyph>> pair : m_glyphs) {
				submit(m_glyphs[pair.first], *pair.first);
			}
		}
		if (m_sprites.empty()) {
			return;
		}
		std::stable_sort(m_sprites.begin(), m_sprites.end(),
			CompareSpriteDepth);
		glBindVertexArray(m_VAOID);
		std::vector<unsigned int> indices;
		std::vector<float> vertices;
		unsigned int count = 0;
		GLSLShader* currentShader = m_sprites.front().first;
		currentShader->begin();
		GLuint currentTexture = getTexture(
			m_sprites.front().second->getTextureFileName());
		glBindTexture(GL_TEXTURE_2D, currentTexture);
		for (const std::pair<GLSLShader*, Sprite*>& pair : m_sprites) {
			if (currentShader != pair.first) {
				if (count > 0) {
					draw(indices, vertices);
					indices.clear();
					vertices.clear();
					count = 0;
				}
				currentShader->end();
				currentShader = pair.first;
				currentShader->begin();
			}
			if (currentTexture != getTexture(
				pair.second->getTextureFileName())) {
				if (count > 0) {
					draw(indices, vertices);
					indices.clear();
					vertices.clear();
					count = 0;
				}
				currentTexture = getTexture(pair.second->getTextureFileName());
				glBindTexture(GL_TEXTURE_2D, currentTexture);
			}
			for (unsigned int index : pair.second->getIndices()) {
				indices.push_back(index + 4 * count);
			}
			for (float vertex : pair.second->getVertices()) {
				vertices.push_back(vertex);
			}
			count++;
		}
		if (count > 0) {
			draw(indices, vertices);
		}
	}

	bool GraphicsManager::destroy() {
		ACTIASFW_LOG(Graphics, Info, "Destroying graphics manager");
		if (!m_initialized) {
			ACTIASFW_LOG(Graphics, Info, "Graphics manager has already been ",
				"destroyed");
			return false;
		}
		ACTIASFW_LOG(Graphics, Info, "Freeing OpenGL buffers");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_IBOID);
		m_IBOID = 0;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_VBOID);
		m_VBOID = 0;
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_VAOID);
		m_VAOID = 0;
		ACTIASFW_LOG(Graphics, Info, "Freeing ", m_textures.size(),
			" textures");
		for (const std::pair<const std::string, GLuint>& texture : m_textures) {
			glDeleteTextures(1, &texture.second);
		}
		m_textures.clear();
		m_sprites.clear();
		ACTIASFW_LOG(Graphics, Info, "Destroying FreeType library");
		FT_Done_FreeType(m_freeType);
		m_freeType = 0;
		ACTIASFW_LOG(Graphics, Info, "Freeing ", m_fonts.size(), " fonts");
		for (std::map<std::string, Font>::iterator it = m_fonts.begin();
			it != m_fonts.end(); it++) {
			it->second.destroy();
		}
		m_fonts.clear();
		m_glyphs.clear();
		m_defaultCamera.destroy();
		m_defaultShader.destroy();
		m_initialized = false;
		return true;
	}

	Camera& GraphicsManager::getDefaultCamera() {
		return m_defaultCamera;
	}

	GLSLShader& GraphicsManager::getDefaultShader() {
		return m_defaultShader;
	}

	const GLuint GraphicsManager::getTexture(const std::string& fileName) {
		if (fileName.empty()) {
			return 0;
		}
		if (m_textures.find(fileName) != m_textures.end()) {
			return m_textures[fileName];
		}
		std::ifstream file(fileName, std::ios::in | std::ios::binary
			| std::ios::ate);
		if (!file.good()) {
			return 0;
		}
		std::streamsize fileSize = 0;
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		fileSize -= file.tellg();
		if (fileSize <= 0) {
			return 0;
		}
		std::vector<unsigned char> buffer;
		buffer.resize((size_t)fileSize);
		file.read((char*)(&buffer[0]), fileSize);
		file.close();
		std::vector<unsigned char> textureData;
		unsigned int width = 0;
		unsigned int height = 0;
		if (lodepng::decode(textureData, width, height, buffer)) {
			return 0;
		}
		GLuint textureID = 0;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, &textureData[0]);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_textures[fileName] = textureID;
		return textureID;
	}

	const Font* GraphicsManager::getFont(const std::string& fileName) {
		if (m_fonts.find(fileName) != m_fonts.end()) {
			return &m_fonts.at(fileName);
		}
		FT_Face face;
		if (FT_New_Face(m_freeType, fileName.c_str(), 0, &face) != FT_Err_Ok) {
			return 0;
		}
		FT_Set_Pixel_Sizes(face, 0, 64);
		Font font;
		for (unsigned char g = 0; g < 128; g++) {
			if (FT_Load_Char(face, g, FT_LOAD_RENDER) != FT_Err_Ok) {
				continue;
			}
			GLuint textureID;
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
				face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer);
			Glyph glyph;
			glyph.setDimensions(glm::vec2(face->glyph->bitmap.width,
				face->glyph->bitmap.rows));
			glyph.setBearing(glm::ivec2(face->glyph->bitmap_left,
				face->glyph->bitmap_top));
			glyph.setOffset(face->glyph->advance.x >> 6);
			glyph.setTextureFileName(fileName + "-" + (char)g);
			m_textures[glyph.getTextureFileName()] = textureID;
			font.addGlyph(g, glyph);
		}
		FT_Done_Face(face);
		m_fonts[fileName] = font;
		return &m_fonts.at(fileName);
	}

	void GraphicsManager::draw(const std::vector<unsigned int>& indices,
		const std::vector<float>& vertices) {
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(),
			vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(unsigned int) * indices.size(), indices.data(),
			GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT,
			(void*)0);
	}
}