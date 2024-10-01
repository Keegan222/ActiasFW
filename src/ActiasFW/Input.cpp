/*
File:		Input.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.07@21:40
Purpose:	Defines variables and implements functions found in Input.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement InputListener class functions

	void InputListener::keyPressed(KeyCode) {

	}

	void InputListener::keyReleased(KeyCode) {

	}

	void InputListener::characterTyped(char) {

	}

	void InputListener::mouseButtonPressed(MouseButtonCode) {

	}

	void InputListener::mouseButtonReleased(MouseButtonCode) {

	}

	void InputListener::mouseCursorMoved(const glm::vec2&) {

	}

	void InputListener::mouseScrolled(const glm::vec2&) {

	}

	void InputListener::controllerConnected(Controller) {

	}

	void InputListener::controllerDisconnected(Controller) {

	}

	void InputListener::controllerButtonPressed(Controller,
		ControllerButtonCode) {

	}

	void InputListener::controllerButtonReleased(Controller,
		ControllerButtonCode) {

	}

	void InputListener::controllerAxisMoved(Controller, ControllerAxisCode,
		float) {

	}

	// Implement InputManager class functions

	bool InputManager::initialize() {
		ACTIASFW_LOG(Input, Info, "Initializing the user input manager");
		if (m_initialized) {
			ACTIASFW_LOG(Input, Error, "The input manager has already been ",
				"initialized");
			return false;
		}
		ACTIASFW_LOG(Input, Info, "Setting GLFW callback functions");
		glfwSetKeyCallback(Application::Window.getHandle(), KeyEvent);
		glfwSetCharCallback(Application::Window.getHandle(), CharacterEvent);
		glfwSetMouseButtonCallback(Application::Window.getHandle(),
			MouseButtonEvent);
		glfwSetCursorPosCallback(Application::Window.getHandle(),
			MousePositionEvent);
		glfwSetScrollCallback(Application::Window.getHandle(),
			MouseScrollEvent);
		glfwSetJoystickCallback(ControllerEvent);
		ACTIASFW_LOG(Input, Info, "Detecting pre-connected controllers");
		for (int i = 0; i < CONTROLLER_LAST_ITERATE; i++) {
			if (glfwJoystickPresent(i) == GLFW_TRUE
				&& glfwJoystickIsGamepad(i) == GLFW_TRUE) {
				connectController((Controller)i);
			}
		}
		setMouseEnabled(true);
		setMouseVisible(true);
		m_initialized = true;
		return true;
	}

	void InputManager::update() {
		for (std::map<KeyCode, std::pair<bool, bool>>::iterator it
			= m_keys.begin(); it != m_keys.end(); it++) {
			it->second.second = it->second.first;
		}
		for (std::map<MouseButtonCode, std::pair<bool, bool>>::iterator it
			= m_mouseButtons.begin(); it != m_mouseButtons.end(); it++) {
			it->second.second = it->second.first;
		}
		m_mousePosition.second = m_mousePosition.first;
		m_mouseScroll = glm::vec2(0.0f, 0.0f);
		glfwPollEvents();
		std::scoped_lock(m_removedControllersMutex);
		while (!m_removedControllers.empty()) {
			Controller removedController = m_removedControllers.front();
			m_removedControllers.pop_front();
			std::map<Controller,
				std::pair<GLFWgamepadstate, GLFWgamepadstate>>::iterator it
				= m_controllers.find(removedController);
			if (it != m_controllers.end()) {
				m_controllers.erase(it);
			}
		}
		for (std::map<Controller,
			std::pair<GLFWgamepadstate, GLFWgamepadstate>>::iterator it
			= m_controllers.begin(); it != m_controllers.end(); it++) {
			if (!glfwJoystickPresent((int)it->first)) {
				continue;
			}
			it->second.second = it->second.first;
			glfwGetGamepadState((int)it->first, &it->second.first);
			for (int b = 0; b <= CONTROLLER_BUTTON_LAST_ITERATE; b++) {
				if (it->second.first.buttons[b]
					!= it->second.second.buttons[b]) {
					if (it->second.first.buttons[b] == GLFW_PRESS) {
						for (InputListener* listener : m_listeners) {
							listener->controllerButtonPressed(it->first,
								(ControllerButtonCode)b);
						}
					}
					else {
						for (InputListener* listener : m_listeners) {
							listener->controllerButtonReleased(it->first,
								(ControllerButtonCode)b);
						}
					}
				}
			}
			for (int a = 0; a <= CONTROLLER_AXIS_LAST_ITERATE; a++) {
				if (it->second.first.axes[a] != it->second.second.axes[a]) {
					for (InputListener* listener : m_listeners) {
						listener->controllerAxisMoved(it->first,
							(ControllerAxisCode)a, it->second.first.axes[a]
								- it->second.second.axes[a]);
					}
				}
			}
		}
	}

	bool InputManager::addListener(InputListener& listener) {
		ACTIASFW_LOG(Input, Info, "Adding input listener at ", &listener);
		if (std::find(m_listeners.begin(), m_listeners.end(), &listener)
			!= m_listeners.end()) {
			ACTIASFW_LOG(Input, Warning, "Listener at ", &listener,
				" is already in memory");
			return false;
		}
		m_listeners.push_back(&listener);
		return true;
	}

	bool InputManager::removeListener(InputListener& listener) {
		ACTIASFW_LOG(Input, Info, "Removing input listener at ", &listener);
		std::vector<InputListener*>::iterator it
			= std::find(m_listeners.begin(), m_listeners.end(), &listener);
		if (it == m_listeners.end()) {
			ACTIASFW_LOG(Input, Warning, "Listener at ", &listener,
				" not found in memory");
			return false;
		}
		m_listeners.erase(it);
		return true;
	}

	bool InputManager::destroy() {
		ACTIASFW_LOG(Input, Info, "Destroying input manager");
		if (!m_initialized) {
			ACTIASFW_LOG(Info, Warning, "The input manager was already ",
				"destroyed");
			return false;
		}
		m_listeners.clear();
		m_keys.clear();
		m_mouseEnabled = false;
		m_mouseVisible = false;
		m_mouseButtons.clear();
		m_mousePosition = std::pair<glm::vec2, glm::vec2>();
		m_mouseScroll = glm::vec2();
		m_controllers.clear();
		std::scoped_lock(m_removedControllersMutex);
		m_removedControllers.clear();
		m_initialized = false;
		return true;
	}

	bool InputManager::isKeyDown(KeyCode key) const {
		if (m_keys.find(key) == m_keys.end()) {
			return false;
		}
		return m_keys.at(key).first;
	}

	bool InputManager::isKeyPressed(KeyCode key) const {
		return isKeyDown(key) && !wasKeyDown(key);
	}

	bool InputManager::isKeyReleased(KeyCode key) const {
		return !isKeyDown(key) && wasKeyDown(key);
	}

	bool InputManager::isMouseEnabled() const {
		return m_mouseEnabled;
	}

	void InputManager::setMouseEnabled(bool enabled) {
		if (enabled && !m_mouseEnabled) {
			ACTIASFW_LOG(Input, Info, "Setting mouse cursor enabled");
			if (m_mouseVisible) {
				glfwSetInputMode(Application::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_NORMAL);
			}
			else {
				glfwSetInputMode(Application::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_HIDDEN);
			}
		}
		else if (!enabled && m_mouseEnabled) {
			ACTIASFW_LOG(Input, Info, "Setting mouse cursor disabled");
			glfwSetInputMode(Application::Window.getHandle(), GLFW_CURSOR,
				GLFW_CURSOR_DISABLED);
		}
		m_mouseEnabled = enabled;
	}

	bool InputManager::isMouseVisible() const {
		return m_mouseVisible;
	}

	void InputManager::setMouseVisible(bool visible) {
		if (m_mouseEnabled) {
			if (visible && !m_mouseVisible) {
				ACTIASFW_LOG(Input, Info, "Setting mouse cursor visible");
				glfwSetInputMode(Application::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_NORMAL);
			}
			else if (!visible && m_mouseVisible) {
				ACTIASFW_LOG(Input, Info, "Setting mouse cursor invisible");
				glfwSetInputMode(Application::Window.getHandle(), GLFW_CURSOR,
					GLFW_CURSOR_HIDDEN);
			}
		}
		m_mouseVisible = visible;
	}

	bool InputManager::isMouseButtonDown(MouseButtonCode button) const {
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			return false;
		}
		return m_mouseButtons.at(button).first;
	}

	bool InputManager::isMouseButtonPressed(MouseButtonCode button) const {
		return isMouseButtonDown(button) && !wasMouseButtonDown(button);
	}

	bool InputManager::isMouseButtonReleased(MouseButtonCode button) const {
		return !isMouseButtonDown(button) && wasMouseButtonDown(button);
	}

	bool InputManager::isMouseCursorMoved() const {
		return m_mousePosition.first != m_mousePosition.second;
	}

	const glm::vec2 InputManager::getMouseCursorPosition() const {
		return glm::vec2(m_mousePosition.first.x,
			Application::Window.getDimensions().y - m_mousePosition.first.y);
	}

	glm::vec2 InputManager::getMouseCursorPosition(Camera& camera) const {
		glm::vec2 mp = getMouseCursorPosition();
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		mp.x -= wd.x / 2.0f;
		mp.y -= wd.y / 2.0f;
		mp /= camera.getScale();
		mp.x += camera.getPosition().x;
		mp.y += camera.getPosition().y;
		return mp;
	}

	const glm::vec2 InputManager::getMouseCursorMovement() const {
		glm::vec2 move = m_mousePosition.first - m_mousePosition.second;
		move.y = -move.y;
		return move;
	}

	bool InputManager::isMouseScrolled() const {
		return m_mouseScroll != glm::vec2(0.0f, 0.0f);
	}

	const glm::vec2& InputManager::getMouseScrollMovement() const {
		return m_mouseScroll;
	}

	std::vector<Controller> InputManager::getControllers() const {
		std::vector<Controller> controllers;
		for (const std::pair<const Controller,
			std::pair<GLFWgamepadstate, GLFWgamepadstate>>& pair
			: m_controllers) {
			controllers.push_back(pair.first);
		}
		return controllers;
	}

	bool InputManager::isControllerConnected(Controller controller) const {
		return m_controllers.find(controller) != m_controllers.end();
	}

	bool InputManager::isControllerButtonDown(Controller controller,
		ControllerButtonCode button) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return false;
		}
		return m_controllers.at(controller).first.buttons[(int)button]
			== GLFW_PRESS;
	}

	bool InputManager::isControllerButtonPressed(Controller controller,
		ControllerButtonCode button) const {
		return isControllerButtonDown(controller, button)
			&& !wasControllerButtonDown(controller, button);
	}

	bool InputManager::isControllerButtonReleased(Controller controller,
		ControllerButtonCode button) const {
		return !isControllerButtonDown(controller, button)
			&& wasControllerButtonDown(controller, button);
	}

	bool InputManager::isControllerAxisMoved(Controller controller,
		ControllerAxisCode axis) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return false;
		}
		return m_controllers.at(controller).first.axes[(int)axis]
			!= m_controllers.at(controller).second.axes[(int)axis];
	}

	float InputManager::getControllerAxisPosition(Controller controller,
		ControllerAxisCode axis) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return 0.0f;
		}
		return m_controllers.at(controller).first.axes[(int)axis];
	}

	float InputManager::getControllerAxisMovement(Controller controller,
		ControllerAxisCode axis) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return 0.0f;
		}
		return m_controllers.at(controller).first.axes[(int)axis]
			- m_controllers.at(controller).second.axes[(int)axis];
	}

	bool InputManager::wasKeyDown(KeyCode key) const {
		if (m_keys.find(key) == m_keys.end()) {
			return false;
		}
		return m_keys.at(key).second;
	}

	void InputManager::pressKey(KeyCode key) {
		if (m_keys.find(key) == m_keys.end()) {
			m_keys[key] = std::pair<bool, bool>(true, false);
		}
		else {
			m_keys[key].first = true;
		}
		for (InputListener* listener : m_listeners) {
			listener->keyPressed(key);
		}
	}

	void InputManager::releaseKey(KeyCode key) {
		if (m_keys.find(key) == m_keys.end()) {
			m_keys[key] = std::pair<bool, bool>(false, true);
		}
		else {
			m_keys[key].first = false;
		}
		for (InputListener* listener : m_listeners) {
			listener->keyReleased(key);
		}
	}

	void InputManager::typeCharacter(char character) {
		for (InputListener* listener : m_listeners) {
			listener->characterTyped(character);
		}
	}

	void InputManager::KeyEvent(GLFWwindow* window, int key, int scancode,
		int action, int mods) {
		if (action == GLFW_PRESS) {
			Application::Input.pressKey((KeyCode)key);
		}
		else if (action == GLFW_RELEASE) {
			Application::Input.releaseKey((KeyCode)key);
		}
	}

	void InputManager::CharacterEvent(GLFWwindow* window,
		unsigned int codepoint) {
		Application::Input.typeCharacter((char)codepoint);
	}

	bool InputManager::wasMouseButtonDown(MouseButtonCode button) const {
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			return false;
		}
		return m_mouseButtons.at(button).second;
	}

	void InputManager::pressMouseButton(MouseButtonCode button) {
		if (!m_mouseEnabled) {
			return;
		}
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			m_mouseButtons[button] = std::pair<bool, bool>(true, false);
		}
		m_mouseButtons[button].first = true;
		for (InputListener* listener : m_listeners) {
			listener->mouseButtonPressed(button);
		}
	}

	void InputManager::releaseMouseButton(MouseButtonCode button) {
		if (!m_mouseEnabled) {
			return;
		}
		if (m_mouseButtons.find(button) == m_mouseButtons.end()) {
			m_mouseButtons[button] = std::pair<bool, bool>(false, true);
		}
		m_mouseButtons[button].first = false;
		for (InputListener* listener : m_listeners) {
			listener->mouseButtonReleased(button);
		}
	}

	void InputManager::moveMouseCursor(const glm::vec2& position) {
		if (!m_mouseEnabled) {
			return;
		}
		m_mousePosition.first = position;
		for (InputListener* listener : m_listeners) {
			listener->mouseCursorMoved(getMouseCursorMovement());
		}
	}

	void InputManager::scrollMouse(const glm::vec2& distance) {
		if (!m_mouseEnabled) {
			return;
		}
		m_mouseScroll = distance;
		for (InputListener* listener : m_listeners) {
			listener->mouseScrolled(distance);
		}
	}

	void InputManager::MouseButtonEvent(GLFWwindow* window, int button,
		int action, int mods) {
		if (action == GLFW_PRESS) {
			Application::Input.pressMouseButton((MouseButtonCode)button);
		}
		else if (action == GLFW_RELEASE) {
			Application::Input.releaseMouseButton((MouseButtonCode)button);
		}
	}

	void InputManager::MousePositionEvent(GLFWwindow* window, double x,
		double y) {
		Application::Input.moveMouseCursor(glm::vec2(x, y));
	}

	void InputManager::MouseScrollEvent(GLFWwindow* window, double x,
		double y) {
		Application::Input.scrollMouse(glm::vec2(x, y));
	}

	bool InputManager::wasControllerButtonDown(Controller controller,
		ControllerButtonCode button) const {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return false;
		}
		return m_controllers.at(controller).second.buttons[(int)button]
			== GLFW_PRESS;
	}

	void InputManager::connectController(Controller controller) {
		if (!glfwJoystickIsGamepad((int)controller)) {
			return;
		}
		if (m_controllers.find(controller) != m_controllers.end()) {
			return;
		}
		ACTIASFW_LOG(Input, Info, "Connecting controller ", controller);
		m_controllers[controller]
			= std::pair<GLFWgamepadstate, GLFWgamepadstate>();
		glfwGetGamepadState((int)controller,
			&m_controllers.at(controller).first);
		glfwGetGamepadState((int)controller,
			&m_controllers.at(controller).second);
		for (InputListener* listener : m_listeners) {
			listener->controllerConnected(controller);
		}
	}

	void InputManager::disconnectController(Controller controller) {
		if (m_controllers.find(controller) == m_controllers.end()) {
			return;
		}
		ACTIASFW_LOG(Input, Info, "Disconnecting controller ", controller);
		for (InputListener* listener : m_listeners) {
			listener->controllerDisconnected(controller);
		}
		std::scoped_lock(m_removedControllersMutex);
		m_removedControllers.push_back(controller);
	}

	void InputManager::ControllerEvent(int jid, int event) {
		if (event == GLFW_CONNECTED) {
			Application::Input.connectController((Controller)jid);
		}
		else if (event == GLFW_DISCONNECTED) {
			Application::Input.disconnectController((Controller)jid);
		}
	}
}