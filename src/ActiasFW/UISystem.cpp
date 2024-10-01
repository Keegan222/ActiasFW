/*
File:		UISystem.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.22@19:46
Purpose:	Defines variables and implements functions found in UISystem.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement UIComponent class functions

	UIListener& UIComponent::getParent() {
		return *m_parent;
	}

	void UIComponent::setParent(UIListener& parent) {
		m_parent = &parent;
	}

	unsigned int UIComponent::getID() const {
		return m_ID;
	}

	void UIComponent::setID(unsigned int ID) {
		m_ID = ID;
	}

	bool UIComponent::isEnabled() const {
		return m_enabled;
	}

	void UIComponent::setEnabled(bool enabled) {
		if (m_enabled && !enabled) {
			Application::Input.removeListener(*this);
		}
		else if (!m_enabled && enabled) {
			Application::Input.addListener(*this);
		}
		m_enabled = enabled;
	}

	bool UIComponent::isVisible() const {
		return m_visible;
	}

	void UIComponent::setVisible(bool visible) {
		if (m_visible && !visible) {
			setEnabled(false);
		}
		m_visible = visible;
	}

	UIComponent& UIComponent::getNeighbourLeft() {
		if (m_neighbours[0] != 0) {
			return *(m_neighbours[0]);
		}
		return *this;
	}

	void UIComponent::setNeighbourLeft(UIComponent& neighbourLeft) {
		m_neighbours[0] = &neighbourLeft;
	}

	UIComponent& UIComponent::getNeighbourRight() {
		if (m_neighbours[1] != 0) {
			return *(m_neighbours[1]);
		}
		return *this;
	}

	void UIComponent::setNeighbourRight(UIComponent& neighbourRight) {
		m_neighbours[1] = &neighbourRight;
	}

	UIComponent& UIComponent::getNeighbourBelow() {
		if (m_neighbours[2] != 0) {
			return *(m_neighbours[2]);
		}
		return *this;
	}

	void UIComponent::setNeighbourBelow(UIComponent& neighbourBelow) {
		m_neighbours[2] = &neighbourBelow;
	}

	UIComponent& UIComponent::getNeighbourAbove() {
		if (m_neighbours[3] != 0) {
			return *(m_neighbours[3]);
		}
		return *this;
	}

	void UIComponent::setNeighbourAbove(UIComponent& neighbourAbove) {
		m_neighbours[3] = &neighbourAbove;
	}

	bool UIComponent::isHighlighted() const {
		return m_highlighted;
	}

	void UIComponent::setHighlighted(bool highlighted) {
		m_highlighted = highlighted;
	}

	// Implement UIGroup class functions

	bool UIGroup::initialize(Scene& parent, unsigned int ID,
		const std::string& fontFileName, float textScale,
		const glm::vec4& textColor, Controller controller,
		const std::string& highlightTextureFileName,
		const glm::vec2& highlightMargin) {
		ACTIASFW_LOG(UIGroup, Info, "Initializing UI group ", ID);
		if (m_initialized) {
			ACTIASFW_LOG(UIGroup, Warning, "UI group has already been ",
				"initialized");
			return false;
		}
		setParent(parent);
		setID(ID);
		setShader(Application::Graphics.getDefaultShader());
		setFontFileName(fontFileName);
		setTextScale(textScale);
		setTextColor(textColor);
		setEnabled(true);
		setVisible(true);
		m_lastWindowDimensions = Application::Window.getDimensions();
		setController(controller);
		m_highlightSprite.initialize(glm::vec3(), glm::vec2(),
			highlightTextureFileName);
		setHighlightMargins(highlightMargin);
		m_initialized = true;
		return true;
	}

	void UIGroup::draw() {
		if (!m_visible) {
			return;
		}
		for (UIComponent* component : m_components) {
			if (!component->isVisible()) {
				continue;
			}
			component->draw();
		}
		if (!Application::Input.getControllers().empty()) {
			Application::Graphics.submit(m_highlightSprite, m_shader);
		}
	}

	void UIGroup::processInput() {
		if (!m_enabled) {
			return;
		}
		for (UIComponent* component : m_components) {
			if (!component->isEnabled()) {
				continue;
			}
			component->processInput();
		}
		if (Application::Input.isControllerConnected(m_controller)) {
			bool f = false;
			UIComponent* hcomp = 0;
			for (UIComponent* component : m_components) {
				if (component->isHighlighted()) {
					f = true;
					glm::vec2 cdims(component->getDimensions().x
						* (float)Application::Window.getDimensions().x,
						component->getDimensions().y
						* (float)Application::Window.getDimensions().y);
					glm::vec3 cpos(component->getPosition().x
						* (float)Application::Window.getDimensions().x,
						component->getPosition().y
						* (float)Application::Window.getDimensions().y,
						component->getPosition().z);
					glm::vec2 hdims(cdims.x + m_highlightMargins.x,
						cdims.y + m_highlightMargins.y);
					m_highlightSprite.setDimensions(hdims);
					glm::vec2 cctr(cpos.x + (cdims.x / 2.0f),
						cpos.y + (cdims.y / 2.0f));
					m_highlightSprite.setPosition(glm::vec3(
						cctr.x - (hdims.x / 2.0f), cctr.y - (hdims.y / 2.0f),
						cpos.z - 0.1f));
					hcomp = component;
					break;
				}
			}
			if (!f) {
				m_highlightSprite.setDimensions(glm::vec2(0.0f, 0.0f));
			}
			if (Application::Input.isControllerButtonPressed(m_controller,
				CONTROLLER_BUTTON_LEFT)) {
				if (f) {
					hcomp->setHighlighted(false);
					hcomp->getNeighbourLeft().setHighlighted(true);
				}
				else {
					if (!m_components.empty()) {
						m_initialComponent->setHighlighted(true);
					}
				}
			}
			if (Application::Input.isControllerButtonPressed(m_controller,
				CONTROLLER_BUTTON_RIGHT)) {
				if (f) {
					hcomp->setHighlighted(false);
					hcomp->getNeighbourRight().setHighlighted(true);
				}
				else {
					if (!m_components.empty()) {
						m_initialComponent->setHighlighted(true);
					}
				}
			}
			if (Application::Input.isControllerButtonPressed(m_controller,
				CONTROLLER_BUTTON_DOWN)) {
				if (f) {
					hcomp->setHighlighted(false);
					hcomp->getNeighbourBelow().setHighlighted(true);
				}
				else {
					if (!m_components.empty()) {
						m_initialComponent->setHighlighted(true);
					}
				}
			}
			if (Application::Input.isControllerButtonPressed(m_controller,
				CONTROLLER_BUTTON_UP)) {
				if (f) {
					hcomp->setHighlighted(false);
					hcomp->getNeighbourAbove().setHighlighted(true);
				}
				else {
					if (!m_components.empty()) {
						m_initialComponent->setHighlighted(true);
					}
				}
			}
		}
	}

	void UIGroup::UIEvent(unsigned int componentID, unsigned int eventID) {
		m_parent->UIEvent(m_ID, componentID, eventID);
	}

	void UIGroup::update(float timeStep) {
		for (UIComponent* component : m_components) {
			component->update(timeStep);
		}
		if (m_lastWindowDimensions != Application::Window.getDimensions()) {
			glm::vec2 wd((float)Application::Window.getDimensions().x,
				(float)Application::Window.getDimensions().y);
			glm::vec2 scale(wd.x / (float)m_lastWindowDimensions.x,
				wd.y / (float)m_lastWindowDimensions.y);
			m_lastWindowDimensions = wd;
			for (UIComponent* component : m_components) {
				glm::vec2 dimensions = component->getDimensions();
				dimensions.x *= scale.x;
				dimensions.y *= scale.y;
				component->setDimensions(dimensions);
				glm::vec3 position = component->getPosition();
				position.x *= scale.x;
				position.y *= scale.y;
				component->setPosition(position);
			}
		}
	}

	bool UIGroup::destroy() {
		ACTIASFW_LOG(UIGroup, Info, "Destroying UI group ", m_ID);
		if (!m_initialized) {
			ACTIASFW_LOG(UIGroup, Warning, "UI group already destroyed");
			return false;
		}
		m_parent = 0;
		m_ID = 0;
		for (UIComponent* component : m_components) {
			component->destroy();
		}
		m_components.clear();
		m_nextID = 0;
		m_fontFileName = "";
		m_textScale = 0.0f;
		m_textColor = glm::vec4();
		m_enabled = false;
		m_visible = false;
		m_lastWindowDimensions = glm::ivec2();
		m_initialized = false;
		return true;
	}

	Scene& UIGroup::getParent() {
		return *m_parent;
	}

	void UIGroup::setParent(Scene& parent) {
		m_parent = &parent;
	}

	unsigned int UIGroup::getID() const {
		return m_ID;
	}

	void UIGroup::setID(unsigned int ID) {
		m_ID = ID;
	}

	bool UIGroup::addComponent(UIComponent& component) {
		ACTIASFW_LOG(UIGroup, Info, "Adding component ", &component,
			" to UI group ", m_ID);
		if (std::find(m_components.begin(), m_components.end(), &component)
			!= m_components.end()) {
			ACTIASFW_LOG(UIGroup, Warning, "Component is already in this group");
			return false;
		}
		component.setParent(*this);
		component.setID(m_nextID++);
		component.setEnabled(m_enabled);
		component.setVisible(m_visible);
		if (m_components.empty()) {
			setInitialComponent(component);
		}
		m_components.push_back(&component);
		return true;
	}

	bool UIGroup::removeComponent(UIComponent& component) {
		ACTIASFW_LOG(UIGroup, Info, "Removing component ", &component,
			" from UI group ", m_ID);
		std::vector<UIComponent*>::iterator it
			= std::find(m_components.begin(), m_components.end(), &component);
		if (it == m_components.end()) {
			ACTIASFW_LOG(UIGroup, Warning, "Component is not in this group");
			return false;
		}
		m_components.erase(it);
		return true;
	}

	GLSLShader& UIGroup::getShader() {
		return m_shader;
	}

	void UIGroup::setShader(const GLSLShader& shader) {
		m_shader = shader;
	}

	const std::string& UIGroup::getFontFileName() const {
		return m_fontFileName;
	}

	void UIGroup::setFontFileName(const std::string& fontFileName) {
		m_fontFileName = fontFileName;
	}

	float UIGroup::getTextScale() const {
		return m_textScale;
	}

	void UIGroup::setTextScale(float textScale) {
		m_textScale = textScale;
	}

	const glm::vec4& UIGroup::getTextColor() const {
		return m_textColor;
	}

	void UIGroup::setTextColor(const glm::vec4& textColor) {
		m_textColor = textColor;
	}

	bool UIGroup::isEnabled() const {
		return m_enabled;
	}

	void UIGroup::setEnabled(bool enabled) {
		m_enabled = enabled;
		for (UIComponent* component : m_components) {
			component->setEnabled(enabled);
		}
	}

	bool UIGroup::isVisible() const {
		return m_visible;
	}

	void UIGroup::setVisible(bool visible) {
		m_visible = visible;
		for (UIComponent* component : m_components) {
			component->setVisible(visible);
		}
	}

	Controller UIGroup::getController() const {
		return m_controller;
	}

	void UIGroup::setController(Controller controller) {
		m_controller = controller;
	}

	UIComponent& UIGroup::getInitialComponent() {
		return *m_initialComponent;
	}

	void UIGroup::setInitialComponent(UIComponent& initialComponent) {
		m_initialComponent = &initialComponent;
	}

	Sprite& UIGroup::getHighlightSprite() {
		return m_highlightSprite;
	}

	void UIGroup::setHighlightSprite(const Sprite& highlightSprite) {
		m_highlightSprite = highlightSprite;
	}

	const glm::vec2& UIGroup::getHighlightMargins() const {
		return m_highlightMargins;
	}

	void UIGroup::setHighlightMargins(const glm::vec2& highlightMargins) {
		m_highlightMargins = highlightMargins;
	}
}