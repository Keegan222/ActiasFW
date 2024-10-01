/*
File:		Windowing.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.06@19:34
Purpose:	Contains GLFW window management utility for ActiasFW applications
*/

#ifndef ACTIASFW_WINDOWING_H
#define ACTIASFW_WINDOWING_H

#include <glm/glm.hpp>
#include <string>
#include <GLFW/glfw3.h>

namespace ActiasFW {
	// The GLFW window management system for ActiasFW applications
	class WindowManager {
	public:
		/*
		Initialize the GLFW library and open a window with given parameters
		Parameter: const glm::ivec2& dimensions - The initial dimensions of this
		window in pixels (width, height)
		Parameter: const std::string& title - The initial title to appear at the
		top of this window
		Parameter: bool fullscreen - Whether this window will initially appear
		in fullscreen mode
		Parameter: unsigned int targetUpdatesPerSecond - The initial desired
		number of logic updates per second
		Parameter: unsigned int maximumUpdatesPerFrame - The initial maximum
		number of logic updates per graphical frame
		Returns: bool - Whether the GLFW library was successfully initialized
		and the window was opened successfully
		*/
		bool initialize(const glm::ivec2&, const std::string&, bool,
			unsigned int, unsigned int);
		/*
		Update this window's graphics buffer in GLFW
		Returns: bool - Whether this window should remain open
		*/
		bool update();
		/*
		Close the GLFW window, terminate the GLFW library, and free this window
		manager's memory
		Returns: bool - Whether this window was successfully destroyed
		*/
		bool destroy();
		/*
		Get the current dimensions of this window
		Returns: const glm::ivec2& - The dimensions of this window in pixels
		(width, height)
		*/
		const glm::ivec2& getDimensions() const;
		/*
		Set the dimensions of this window when not in fullscreen mode
		Parameter: const glm::ivec2& dimensions - The new dimensions of this
		window in pixels (width, height)
		*/
		void setDimensions(const glm::ivec2&);
		/*
		Get the current title of this window
		Returns: const std::string& - The title of this window
		*/
		const std::string& getTitle() const;
		/*
		Set the title of this window
		Parameter: const std::string& title - The new title of this window
		*/
		void setTitle(const std::string&);
		/*
		Test whether this window is currently in fullscreen mode
		Returns: bool - Whether this window is in fullscreen mode
		*/
		bool isFullscreen() const;
		/*
		Set whether this window is in fullscreen mode or windowed mode
		Parameter: bool fullscreen - The new fullscreen value for this window
		*/
		void setFullscreen(bool);
		/*
		Get the monitor this window appears on when in fullscreen mode
		Returns: GLFWmonitor* - This window's monitor
		*/
		GLFWmonitor* getMonitor() const;
		/*
		Set the monitor this window should appear on when in fullscreen mode
		Parameter: GLFWmonitor* monitor - This window's new monitor
		*/
		void setMonitor(GLFWmonitor*);
		/*
		Get the GLFW handle for this window
		Returns: GLFWwindow* - The GLFW handle for this window
		*/
		GLFWwindow* getHandle() const;
		/*
		Set the GLFW handle for this window
		Parameter: GLFWwindow* handle - The new GLFW handle for this window
		*/
		void setHandle(GLFWwindow*);
		/*
		Get the target number of logic updates per second for this window
		Returns: unsigned int - The target number of logic updates per second
		*/
		unsigned int getTargetUpdatesPerSecond() const;
		/*
		Set the target number of logic updates per second for this window
		Parameter: unsigned int targetUpdatesPerSecond - The new target number
		of logic updates per second
		*/
		void setTargetUpdatesPerSecond(unsigned int);
		/*
		Get the maximum number of logic updates per graphics frame rendered
		Returns: unsigned int - The maximum number of logic updates per frame
		*/
		unsigned int getMaximumUpdatesPerFrame() const;
		/*
		Set the maximum number of logic updates per graphics frame rendered
		Parameter: unsigned int maximumUpdatesPerSecond - The new maximum
		number of logic updates per frame
		*/
		void setMaximumUpdatesPerFrame(unsigned int);

	private:
		// Whether this window manager has been initialized
		bool m_initialized = false;
		// The current dimensions of this window
		glm::ivec2 m_dimensions = glm::ivec2();
		// The dimensions of this window when in windowed mode
		glm::ivec2 m_windowedDimensions = glm::ivec2();
		// The title of this window
		std::string m_title = "";
		// Whether this window is currently in fullscreen mode
		bool m_fullscreen = false;
		// The monitor this window appears on when in fullscreen mode
		GLFWmonitor* m_monitor = 0;
		// The GLFW handle for this window
		GLFWwindow* m_handle = 0;
		// The target number of logic updates per second in this window
		unsigned int m_targetUpdatesPerSecond = 0;
		// The maximum number of logic updates per graphics frame rendered in
		// this window
		unsigned int m_maximumUpdatesPerFrame = 0;

		/*
		Move this window to the center of its current monitor
		*/
		void center();
	};
}

#endif