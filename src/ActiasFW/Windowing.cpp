/*
File:		Windowing.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.06@19:34
Purpose:	Defines variables and implements functions found in Windowing.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement WindowManager class functions

	bool WindowManager::initialize(const glm::ivec2& dimensions,
		const std::string& title, bool fullscreen,
		unsigned int targetUpdatesPerSecond,
		unsigned int maximumUpdatesPerFrame) {
		ACTIASFW_LOG(Window, Info, "Initializing Actias application window");
		if (m_initialized) {
			ACTIASFW_LOG(Window, Error, "Window already initialized");
			return false;
		}
		ACTIASFW_LOG(Window, Info, "Initializing GLFW library");
		if (!glfwInit()) {
			ACTIASFW_LOG(Window, Error, "Failed to initialize GLFW library");
			return false;
		}
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		setHandle(glfwCreateWindow(1, 1, "", 0, 0));
		glfwMakeContextCurrent(m_handle);
		setMonitor(glfwGetPrimaryMonitor());
		setDimensions(dimensions);
		setTitle(title);
		setFullscreen(fullscreen);
		center();
		setTargetUpdatesPerSecond(targetUpdatesPerSecond);
		setMaximumUpdatesPerFrame(maximumUpdatesPerFrame);
		m_initialized = true;
		return true;
	}

	bool WindowManager::update() {
		glfwSwapBuffers(m_handle);
		return !glfwWindowShouldClose(m_handle);
	}

	bool WindowManager::destroy() {
		ACTIASFW_LOG(Window, Info, "Destroying window manager");
		if (!m_initialized) {
			ACTIASFW_LOG(Window, Warning, "The window was already destroyed");
			return false;
		}
		ACTIASFW_LOG(Window, Info, "Destroying GLFW window");
		glfwDestroyWindow(m_handle);
		ACTIASFW_LOG(Window, Info, "Terminating GLFW library");
		glfwTerminate();
		ACTIASFW_LOG(Window, Info, "Freeing ActiasFW application window memory");
		m_initialized = false;
		m_dimensions = glm::ivec2();
		m_windowedDimensions = glm::ivec2();
		m_title = "";
		m_fullscreen = false;
		m_monitor = 0;
		m_handle = 0;
		return true;
	}

	const glm::ivec2& WindowManager::getDimensions() const {
		return m_dimensions;
	}

	void WindowManager::setDimensions(const glm::ivec2& dimensions) {
		ACTIASFW_LOG(Window, Info, "Setting window dimensions to (",
			dimensions.x, ", ", dimensions.y, ")");
		m_windowedDimensions = dimensions;
		if (m_fullscreen) {
			ACTIASFW_LOG(Window, Warning, "Window is in fullscreen mode");
			return;
		}
		m_dimensions = dimensions;
		glfwSetWindowSize(m_handle, dimensions.x, dimensions.y);
		glViewport(0, 0, dimensions.x, dimensions.y);
		center();
	}

	const std::string& WindowManager::getTitle() const {
		return m_title;
	}

	void WindowManager::setTitle(const std::string& title) {
		ACTIASFW_LOG(Window, Info, "Setting window title to \"", title, "\"");
		m_title = title;
		glfwSetWindowTitle(m_handle, title.c_str());
	}

	bool WindowManager::isFullscreen() const {
		return m_fullscreen;
	}

	void WindowManager::setFullscreen(bool fullscreen) {
		if (fullscreen && !m_fullscreen) {
			ACTIASFW_LOG(Window, Info, "Setting window to fullscreen mode");
			const GLFWvidmode* vm = glfwGetVideoMode(m_monitor);
			glfwSetWindowMonitor(m_handle, m_monitor, 0, 0, vm->width,
				vm->height, GLFW_DONT_CARE);
			glViewport(0, 0, vm->width, vm->height);
			m_windowedDimensions = m_dimensions;
			m_dimensions = glm::ivec2(vm->width, vm->height);
		}
		else if (!fullscreen && m_fullscreen) {
			ACTIASFW_LOG(Window, Info, "Setting window to windowed mode");
			glfwSetWindowMonitor(m_handle, 0, 0, 0, m_windowedDimensions.x,
				m_windowedDimensions.y, GLFW_DONT_CARE);
			glfwSetWindowAttrib(m_handle, GLFW_DECORATED, GLFW_TRUE);
			glViewport(0, 0, m_windowedDimensions.x, m_windowedDimensions.y);
			m_dimensions = m_windowedDimensions;
			center();
		}
		m_fullscreen = fullscreen;
	}

	GLFWmonitor* WindowManager::getMonitor() const {
		return m_monitor;
	}

	void WindowManager::setMonitor(GLFWmonitor* monitor) {
		ACTIASFW_LOG(Window, Info, "Setting window monitor to ", monitor);
		bool fullscreen = m_fullscreen;
		if (fullscreen) {
			setFullscreen(false);
		}
		m_monitor = monitor;
		if (fullscreen) {
			setFullscreen(true);
		}
	}

	GLFWwindow* WindowManager::getHandle() const {
		return m_handle;
	}

	void WindowManager::setHandle(GLFWwindow* handle) {
		ACTIASFW_LOG(Window, Info, "Setting GLFW window handle to ", handle);
		m_handle = handle;
	}

	unsigned int WindowManager::getTargetUpdatesPerSecond() const {
		return m_targetUpdatesPerSecond;
	}

	void WindowManager::setTargetUpdatesPerSecond(
		unsigned int targetUpdatesPerSecond) {
		m_targetUpdatesPerSecond = targetUpdatesPerSecond;
	}

	unsigned int WindowManager::getMaximumUpdatesPerFrame() const {
		return m_maximumUpdatesPerFrame;
	}

	void WindowManager::setMaximumUpdatesPerFrame(
		unsigned int maximumUpdatesPerFrame) {
		m_maximumUpdatesPerFrame = maximumUpdatesPerFrame;
	}

	void WindowManager::center() {
		const GLFWvidmode* vm = glfwGetVideoMode(m_monitor);
		glfwSetWindowPos(m_handle, (vm->width - m_dimensions.x) / 2,
			(vm->height - m_dimensions.y) / 2);
	}
}