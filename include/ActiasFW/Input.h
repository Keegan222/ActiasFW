/*
File:		Input.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.07@21:39
Purpose:	Contains user input management utilities for ActiasFW applications
*/

#ifndef ACTIASFW_INPUT_H
#define ACTIASFW_INPUT_H

#include "InputCodes.h"

#include <vector>
#include <map>
#include <deque>
#include <mutex>

namespace ActiasFW {
	// Abstract class for callback functions for user input events
	class InputListener {
	public:
		/*
		A key has been pressed on the keyboard
		Parameter: KeyCode - The key pressed
		*/
		virtual void keyPressed(KeyCode);
		/*
		A key has been released on the keyboard
		Parameter: KeyCode - The key released
		*/
		virtual void keyReleased(KeyCode);
		/*
		A character has been typed on the keyboard
		Parameter: char - The character typed
		*/
		virtual void characterTyped(char);
		/*
		A button has been pressed on the mouse
		Parameter: MouseButtonCode - The button pressed
		*/
		virtual void mouseButtonPressed(MouseButtonCode);
		/*
		A button has been released on the mouse
		Parameter: MouseButtonCode - The button released
		*/
		virtual void mouseButtonReleased(MouseButtonCode);
		/*
		The mouse cursor has moved
		Parameter: const glm::vec2& - The distance the cursor moved in pixels
		*/
		virtual void mouseCursorMoved(const glm::vec2&);
		/*
		The mouse's scroll wheel has moved
		Parameter: const glm::vec2& - The distance the scroll wheel moved
		*/
		virtual void mouseScrolled(const glm::vec2&);
		/*
		A game controller has been connected
		Parameter: Controller - The controller connected
		*/
		virtual void controllerConnected(Controller);
		/*
		A game controller has been disconnected
		Parameter: Controller - The controller disconnected
		*/
		virtual void controllerDisconnected(Controller);
		/*
		A button has been pressed on a game controller
		Parameter: Controller - The controller
		Parameter: ControllerButtonCode - The controller button pressed
		*/
		virtual void controllerButtonPressed(Controller, ControllerButtonCode);
		/*
		A button has been released on a game controller
		Parameter: Controller - The controller
		Parameter: ControllerButtonCode - The controller button released
		*/
		virtual void controllerButtonReleased(Controller, ControllerButtonCode);
		/*
		An axis has moved on a game controller
		Parameter: Controller - The controller
		Parameter: ControllerAxisCode - The controller axis moved
		Parameter: float - The distance the axis moved
		*/
		virtual void controllerAxisMoved(Controller, ControllerAxisCode,
			float);
	};

	// The user input management system for ActiasFW applications
	class InputManager {
	public:
		/*
		Set GLFW input event callback functions and initialize this input
		manager's memory
		Returns: bool - Whether the input manager was initialized successfully
		*/
		bool initialize();
		/*
		Update the state of all user input devices
		*/
		void update();
		/*
		Add an input listener to be notified of input events
		Parameter: InputListener& listener - The input listener to add
		Returns: bool - Whether the input listener could be added
		*/
		bool addListener(InputListener&);
		/*
		Remove an input listener from this input manager
		Parameter: InputListener& listener - The input listener to remove
		Returns: bool - Whether the input listener could be removed
		*/
		bool removeListener(InputListener&);
		/*
		Free this input manager's memory
		Returns: bool - Whether the input manager was successfully destroyed
		*/
		bool destroy();
		/*
		Test if a key on the keyboard is currently down
		Parameter: KeyCode key - The key to test
		Returns: bool - Whether the key is down
		*/
		bool isKeyDown(KeyCode) const;
		/*
		Test if a key on the keyboard was just pressed
		Parameter: KeyCode key - The key to test
		Returns: bool - Whether the key was just pressed
		*/
		bool isKeyPressed(KeyCode) const;
		/*
		Test if a key on the keyboard was just released
		Parameter: KeyCode key - The key to test
		Returns: bool - Whether the key was just released
		*/
		bool isKeyReleased(KeyCode) const;
		/*
		Test whether the mouse is enabled on the application's window
		Returns: bool - Whether the mouse is enabled
		*/
		bool isMouseEnabled() const;
		/*
		Set whether the mouse is enabled on the application's window
		Parameter: bool enabled - Whether to enable the mouse
		*/
		void setMouseEnabled(bool);
		/*
		Test whether the mouse is visible on the application's window
		Returns: bool - Whether the mouse is visible
		*/
		bool isMouseVisible() const;
		/*
		Set whether the mouse is enabled on the application's window
		Parameter: bool visible - Whether to make the mouse visible
		*/
		void setMouseVisible(bool);
		/*
		Test whether a button is currently down on the mouse
		Parameter: MouseButtonCode button - The mouse button to test
		Returns: bool - Whether the mouse button is down
		*/
		bool isMouseButtonDown(MouseButtonCode) const;
		/*
		Test whether a button has just been pressed on the mouse
		Parameter: MouseButtonCode button - The mouse button to test
		Returns: bool - Whether the mouse button was just pressed
		*/
		bool isMouseButtonPressed(MouseButtonCode) const;
		/*
		Test whether a button has just been released on the mouse
		Parameter: MouseButtonCode button - The mouse button to test
		Returns: bool - Whether the mouse button was just released
		*/
		bool isMouseButtonReleased(MouseButtonCode) const;
		/*
		Test whether the mouse cursor has just moved
		Returns: bool - Whether the mouse cursor just moved
		*/
		bool isMouseCursorMoved() const;
		/*
		Get the current position of the mouse cursor on the window
		Returns: const glm::vec2 - The current position of the mouse cursor
		*/
		const glm::vec2 getMouseCursorPosition() const;
		/*
		Get the current position of the mouse cursor in the perspective of a
		camera
		Parameter: Camera& camera - The camera to test the mouse position under
		Returns: glm::vec2 - The position of the mouse cursor
		*/
		glm::vec2 getMouseCursorPosition(Camera&) const;
		/*
		Get the most recent distance moved by the mouse cursor on the window
		Returns: const glm::vec2 - The most recent movement of the mouse cursor
		*/
		const glm::vec2 getMouseCursorMovement() const;
		/*
		Test whether the mouse's scroll wheel has just moved
		Returns: bool - Whether the mouse's scroll wheel just moved
		*/
		bool isMouseScrolled() const;
		/*
		Get the most recent distance moved by the mouse's scroll wheel on the
		window
		Returns: const glm::vec2& - The most recent movement of the mouse's
		scroll wheel
		*/
		const glm::vec2& getMouseScrollMovement() const;
		/*
		Get the set of currently connected game controllers
		Returns: std::vector<Controller> - The set of controllers
		*/
		std::vector<Controller> getControllers() const;
		/*
		Test whether a game controller is currently connected
		Parameter: Controller controller - The controller to test
		Returns: bool - Whether the controller is connected
		*/
		bool isControllerConnected(Controller) const;
		/*
		Test whether a button is currently down on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerButtonCode button - The button to test
		Returns: bool - Whether the button is down on the given controller
		*/
		bool isControllerButtonDown(Controller, ControllerButtonCode) const;
		/*
		Test whether a button has just been pressed on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerButtonCode button - The button to test
		Returns: bool - Whether the button has just been pressed on the given
		controller
		*/
		bool isControllerButtonPressed(Controller, ControllerButtonCode) const;
		/*
		Test whether a button has just been released on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerButtonCode button - The button to test
		Returns: bool - Whether the button has just been released on the given
		controller
		*/
		bool isControllerButtonReleased(Controller, ControllerButtonCode) const;
		/*
		Test whether an axis has just moved on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerAxisCode axis - The axis to test
		Returns: bool - Whether the axis has just moved on the given controller
		*/
		bool isControllerAxisMoved(Controller, ControllerAxisCode) const;
		/*
		Get the current position of an axis on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerAxisCode axis - The axis to test
		Returns: float - The current position of the axis on the given
		controller
		*/
		float getControllerAxisPosition(Controller, ControllerAxisCode) const;
		/*
		Get the most recent distance moved by an axis on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerAxisCode axis - The axis to test
		Returns: float - The most recent movement of the axis on the given
		controller
		*/
		float getControllerAxisMovement(Controller, ControllerAxisCode) const;

	private:
		// Whether this input manager has been initialized
		bool m_initialized = false;
		// The set of input listeners to be notified of input events
		std::vector<InputListener*> m_listeners;
		// The current and last states of keys on the keyboard
		std::map<KeyCode, std::pair<bool, bool>> m_keys;
		// Whether the mouse is enabled on the application's window
		bool m_mouseEnabled = false;
		// Whether the mouse is visible on the application's window
		bool m_mouseVisible = false;
		// The current and last states of the buttons on the mouse
		std::map<MouseButtonCode, std::pair<bool, bool>> m_mouseButtons;
		// The current and last position of the mouse cursor on the
		// application's window
		std::pair<glm::vec2, glm::vec2> m_mousePosition
			= std::pair<glm::vec2, glm::vec2>();
		// The last distance moved by the mouse's scroll wheel
		glm::vec2 m_mouseScroll = glm::vec2();
		// The current and last states of all currently connected game
		// controllers
		std::map<Controller, std::pair<GLFWgamepadstate, GLFWgamepadstate>>
			m_controllers;
		// The set of controllers to remove on the next update
		std::deque<Controller> m_removedControllers;
		// A mutex to keep the set of removed controllers memory safe for
		// synchronous functions
		std::mutex m_removedControllersMutex;

		/*
		Test whether a key was just down on the keyboard
		Parameter: KeyCode key - The key to test
		Returns: bool - Whether the key was just down
		*/
		bool wasKeyDown(KeyCode) const;
		/*
		Set a keyboard key's state to down and notify input listeners
		Parameter: KeyCode key - The key to set
		*/
		void pressKey(KeyCode);
		/*
		Set a keyboard key's state to up and notify input listeners
		Parameter: KeyCode key - The key to set
		*/
		void releaseKey(KeyCode);
		/*
		Notify input listeners that a character has been typed on the keyboard
		Parameter: char character - The character typed
		*/
		void typeCharacter(char);
		/*
		GLFW callback function for keyboard key events
		Parameter: GLFWwindow* window - The GLFW window handle which triggered
		the event
		Parameter: int key - The key code
		Parameter: int scancode - Extra key code data
		Parameter: int action - Whether the key was pressed or released
		Parameter: int mods - Extra key action data
		*/
		static void KeyEvent(GLFWwindow*, int, int, int, int);
		/*
		GLFW callback function for keyboard typing events
		Parameter: GLFWwindow* window - The GLFW window handle which triggered
		the event
		Parameter: unsigned int codepoint - The character data
		*/
		static void CharacterEvent(GLFWwindow*, unsigned int);
		/*
		Test whether a button was just down on the mouse
		Parameter: MouseButtonCode button - The button to test
		Returns: bool - Whether the button was just down
		*/
		bool wasMouseButtonDown(MouseButtonCode) const;
		/*
		Set a mouse button's state to down and notify input listeners
		Parameter: MouseButtonCode button - The button to set
		*/
		void pressMouseButton(MouseButtonCode);
		/*
		Set a mouse button's state to up and notify input listeners
		Parameter: MouseButtonCode button - The button to set
		*/
		void releaseMouseButton(MouseButtonCode);
		/*
		Update the mouse cursor's position on the application's window and
		notify input listeners
		Parameter: const glm::vec2& position - The new position of the cursor
		*/
		void moveMouseCursor(const glm::vec2&);
		/*
		Update the mouse's scroll wheel's most recent movement on the
		application's window and notify input listeners
		Parameter: const glm::vec2& distance - The distance the scroll wheel
		moved
		*/
		void scrollMouse(const glm::vec2&);
		/*
		GLFW callback for mouse button events
		Parameter: GLFWwindow* window - The GLFW window handle which triggered
		the event
		Parameter: int button - The button code
		Parameter: int action - Whether the button was pressed or released
		Parameter: int mods - Extra action data
		*/
		static void MouseButtonEvent(GLFWwindow*, int, int, int);
		/*
		GLFW callback for mouse movement events
		Parameter: GLFWwindow* window - The GLFW window handle which triggered
		the event
		Parameter: double x - The horizontal component of the new position
		Parameter: double y - The vertical component of the new position
		*/
		static void MousePositionEvent(GLFWwindow*, double, double);
		/*
		GLFW callback for mouse scroll wheel events
		Parameter: GLFWwindow* window - The GLFW window handle which triggered
		the event
		Parameter: double x - The horizonal component of the new scroll distance
		Parameter: double y - The vertical component of the new scroll distance
		*/
		static void MouseScrollEvent(GLFWwindow*, double, double);
		/*
		Test whether a button was just down on a game controller
		Parameter: Controller controller - The controller to test
		Parameter: ControllerButtonCode button - The button to test
		Returns: bool - Whether the button was just down
		*/
		bool wasControllerButtonDown(Controller, ControllerButtonCode) const;
		/*
		Connect a game controller and notify input listeners
		Parameter: Controller controller - The controller connected
		*/
		void connectController(Controller);
		/*
		Disconnect a game controller and notify input listeners
		Parameter: Controller controller - The controller disconnected
		*/
		void disconnectController(Controller);
		/*
		GLFW callback for game controller connection and disconnection events
		Parameter: int jid - The controller
		Parameter: int event - Whether the controller was connected or
		disconnected
		*/
		static void ControllerEvent(int, int);
	};
}

#endif