/*
File:		UIComponents.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.22@19:47
Purpose:	Defines variables and implements functions found in UIComponents.h
*/

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement UIComponents module functions

	/*
	Test whether the mouse cursor is on top of a UI component
	Parameter: UIComponent& component - The component to test
	Returns: bool - Whether the mouse cursor is over the given component
	*/
	bool IsMouseOver(UIComponent& component) {
		glm::vec2 mp = Application::Input.getMouseCursorPosition(
			component.getParent().getShader().getCamera());
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		glm::vec3 cp = component.getPosition();
		cp.x *= wd.x;
		cp.y *= wd.y;
		glm::vec2 cd = component.getDimensions();
		cd.x *= wd.x;
		cd.y *= wd.y;
		if (mp.x < cp.x || mp.x > cp.x + cd.x) {
			return false;
		}
		if (mp.y < cp.y || mp.y > cp.y + cd.y) {
			return false;
		}
		return true;
	}

	// Implement UILabel class functions

	void UILabel::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const std::string& labelText, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment) {
		m_backgroundSprite.initialize(position, dimensions, textureFileName);
		setPosition(position);
		setDimensions(dimensions);
		setLabelText(labelText);
		setLabelAlignment(labelAlignment);
		setHorizontalLabelAlignment(horizontalLabelAlignment);
		setVerticalLabelAlignment(verticalLabelAlignment);
	}

	glm::vec3 UILabel::getPosition() const {
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		return glm::vec3(m_backgroundSprite.getPosition().x / wd.x,
			m_backgroundSprite.getPosition().y / wd.y,
			m_backgroundSprite.getPosition().z);
	}

	void UILabel::setPosition(const glm::vec3& position) {
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		m_backgroundSprite.setPosition(glm::vec3(position.x * wd.x,
			position.y * wd.y, position.z));
	}

	glm::vec2 UILabel::getDimensions() const {
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		return glm::vec2(m_backgroundSprite.getDimensions().x / wd.x,
			m_backgroundSprite.getDimensions().y / wd.y);
	}

	void UILabel::setDimensions(const glm::vec2& dimensions) {
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		m_backgroundSprite.setDimensions(glm::vec2(dimensions.x * wd.x,
			dimensions.y * wd.y));
	}

	Sprite& UILabel::getBackgroundSprite() {
		return m_backgroundSprite;
	}

	void UILabel::setBackgroundSprite(const Sprite& backgroundSprite) {
		m_backgroundSprite = backgroundSprite;
	}

	const std::string& UILabel::getLabelText() const {
		return m_labelText;
	}

	void UILabel::setLabelText(const std::string& labelText) {
		m_labelText = labelText;
	}

	Alignment UILabel::getLabelAlignment() const {
		return m_labelAlignment;
	}

	void UILabel::setLabelAlignment(Alignment labelAlignment) {
		m_labelAlignment = labelAlignment;
	}

	Alignment UILabel::getHorizontalLabelAlignment() const {
		return m_horizontalLabelAlignment;
	}

	void UILabel::setHorizontalLabelAlignment(
		Alignment horizontalLabelAlignment) {
		m_horizontalLabelAlignment = horizontalLabelAlignment;
	}

	Alignment UILabel::getVerticalLabelAlignment() const {
		return m_verticalLabelAlignment;
	}

	void UILabel::setVerticalLabelAlignment(
		Alignment verticalLabelAlignment) {
		m_verticalLabelAlignment = verticalLabelAlignment;
	}

	void UILabel::draw() {
		Application::Graphics.submit(m_backgroundSprite, m_parent->getShader());
		glm::vec2 wd((float)Application::Window.getDimensions().x,
			(float)Application::Window.getDimensions().y);
		glm::vec3 textPosition = getPosition();
		textPosition.x *= wd.x;
		textPosition.y *= wd.y;
		textPosition.z += 0.1f;
		glm::vec2 textDimensions = getDimensions();
		textDimensions.x *= wd.x;
		textDimensions.y *= wd.y;
		if (m_labelAlignment == ALIGN_LEFT) {
			textPosition.x -= textDimensions.x;
		}
		else if (m_labelAlignment == ALIGN_RIGHT) {
			textPosition.x += textDimensions.x;
		}
		else if (m_labelAlignment == ALIGN_BOTTOM) {
			textPosition.y -= textDimensions.y;
		}
		else if (m_labelAlignment == ALIGN_TOP) {
			textPosition.y += textDimensions.y;
		}
		glm::vec4 textBox(textPosition.x, textPosition.y, textDimensions.x,
			textDimensions.y);
		Application::Graphics.submit(m_labelText, textPosition, textBox,
			m_parent->getTextScale(), m_parent->getTextColor(),
			m_parent->getFontFileName(), m_horizontalLabelAlignment,
			m_verticalLabelAlignment, m_parent->getShader());
	}

	void UILabel::processInput() {

	}

	void UILabel::update(float) {

	}

	void UILabel::destroy() {
		m_backgroundSprite.destroy();
		m_labelText = "";
		m_labelAlignment = NO_ALIGNMENT;
		m_horizontalLabelAlignment = NO_ALIGNMENT;
		m_verticalLabelAlignment = NO_ALIGNMENT;
	}

	// Define UIButton class static variables

	glm::vec4 UIButton::UnselectedTextureBox = glm::vec4(
		0.0f, 0.0f, 1.0f / 3.0f, 1.0f);
	glm::vec4 UIButton::SelectedTextureBox = glm::vec4(
		1.0f / 3.0f, 0.0f, 1.0f / 3.0f, 1.0f);
	glm::vec4 UIButton::ClickedTextureBox = glm::vec4(
		2.0f / 3.0f, 0.0f, 1.0f / 3.0f, 1.0f);

	// Implement UIButton class functions

	void UIButton::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const std::string& labelText, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment) {
		UILabel::initialize(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setSelected(false);
		setClicked(false);
	}

	bool UIButton::isSelected() const {
		return m_selected;
	}

	void UIButton::setSelected(bool selected) {
		if (selected) {
			m_backgroundSprite.setTextureBox(SelectedTextureBox);
		}
		else {
			m_backgroundSprite.setTextureBox(UnselectedTextureBox);
		}
		m_selected = selected;
	}

	bool UIButton::isClicked() const {
		return m_clicked;
	}

	void UIButton::setClicked(bool clicked) {
		if (clicked) {
			m_backgroundSprite.setTextureBox(ClickedTextureBox);
		}
		else {
			if (m_selected) {
				m_backgroundSprite.setTextureBox(SelectedTextureBox);
			}
			else {
				m_backgroundSprite.setTextureBox(UnselectedTextureBox);
			}
		}
		m_clicked = clicked;
	}

	void UIButton::setEnabled(bool enabled) {
		m_enabled = enabled;
		if (!m_enabled) {
			setSelected(false);
			setClicked(false);
		}
	}

	void UIButton::processInput() {
		if (m_highlighted) {
			if (Application::Input.isControllerButtonPressed(
				m_parent->getController(), CONTROLLER_BUTTON_A)) {
				setClicked(true);
			}
			if (Application::Input.isControllerButtonReleased(
				m_parent->getController(), CONTROLLER_BUTTON_A)) {
				setClicked(false);
				m_parent->UIEvent(m_ID, ClickedEventID);
			}
		}
		if (IsMouseOver(*this)) {
			if (!m_selected) {
				setSelected(true);
				m_parent->UIEvent(m_ID, SelectedEventID);
			}
		}
		else {
			if (m_selected) {
				setSelected(false);
				m_parent->UIEvent(m_ID, UnselectedEventID);
			}
		}
		if (m_selected) {
			if (m_clicked) {
				if (Application::Input.isMouseButtonReleased(
					MOUSE_BUTTON_LEFT)) {
					m_parent->UIEvent(m_ID, ClickedEventID);
				}
			}
			if (Application::Input.isMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				setClicked(true);
			}
			else {
				setClicked(false);
			}
		}
	}

	void UIButton::destroy() {
		UILabel::destroy();
		m_selected = false;
		m_clicked = false;
	}

	// Define UISwitch class static variables

	glm::vec4 UISwitch::OffTextureBox = glm::vec4(
		0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	glm::vec4 UISwitch::OnTextureBox = glm::vec4(
		1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	// Implement UISwitch class functions

	void UISwitch::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const std::string& labelText, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment) {
		UIButton::initialize(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setOn(false);
	}

	bool UISwitch::isOn() const {
		return m_on;
	}

	void UISwitch::setOn(bool on) {
		if (!on) {
			m_backgroundSprite.setTextureBox(OffTextureBox);
		}
		else {
			m_backgroundSprite.setTextureBox(OnTextureBox);
		}
		m_on = on;
	}

	void UISwitch::processInput() {
		if (m_highlighted) {
			if (Application::Input.isControllerButtonPressed(
				m_parent->getController(), CONTROLLER_BUTTON_A)) {
				if (m_on) {
					setOn(false);
					m_parent->UIEvent(m_ID, OffEventID);
				}
				else {
					setOn(true);
					m_parent->UIEvent(m_ID, OnEventID);
				}
			}
		}
		if (IsMouseOver(*this)) {
			if (Application::Input.isMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
				if (m_on) {
					setOn(false);
					m_parent->UIEvent(m_ID, OffEventID);
				}
				else {
					setOn(true);
					m_parent->UIEvent(m_ID, OnEventID);
				}
			}
		}
	}

	void UISwitch::destroy() {
		UIButton::destroy();
		m_on = false;
	}

	// Define UISlider class static variables

	glm::vec4 UISlider::UnselectedTextureBox = glm::vec4(
		0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	glm::vec4 UISlider::SelectedTextureBox = glm::vec4(
		1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	// Implement UISlider class functions

	void UISlider::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& labelText,
		float value, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment,
		Alignment verticalCursorAlignment) {
		UILabel::initialize(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setSelected(false);
		setValue(value);
		m_cursor.initialize(glm::vec3(), cursorDimensions,
			cursorTextureFileName);
		setVerticalCursorAlignment(verticalCursorAlignment);
	}

	bool UISlider::isSelected() const {
		return m_selected;
	}

	void UISlider::setSelected(bool selected) {
		if (!selected) {
			m_backgroundSprite.setTextureBox(UnselectedTextureBox);
		}
		else {
			m_backgroundSprite.setTextureBox(SelectedTextureBox);
		}
		m_selected = selected;
	}

	float UISlider::getValue() const {
		return m_value;
	}

	void UISlider::setValue(float value) {
		if (value < 0.0f) {
			m_value = 0.0f;
		}
		else if (value > 1.0f) {
			m_value = 1.0f;
		}
		else {
			m_value = value;
		}
	}

	Sprite& UISlider::getCursor() {
		return m_cursor;
	}

	void UISlider::setCursor(const Sprite& cursor) {
		m_cursor = cursor;
	}

	Alignment UISlider::getVerticalCursorAlignment() const {
		return m_verticalCursorAlignment;
	}

	void UISlider::setVerticalCursorAlignment(
		Alignment verticalCursorAlignment) {
		m_verticalCursorAlignment = verticalCursorAlignment;
	}

	void UISlider::setEnabled(bool enabled) {
		m_enabled = enabled;
		if (!m_enabled) {
			setSelected(false);
		}
	}

	void UISlider::setDimensions(const glm::vec2& dimensions) {
		glm::vec2 cdims = m_cursor.getDimensions();
		cdims.y *= (dimensions.y / getDimensions().y);
		m_cursor.setDimensions(cdims);
		UILabel::setDimensions(dimensions);
	}

	void UISlider::draw() {
		UILabel::draw();
		Application::Graphics.submit(m_cursor, m_parent->getShader());
	}

	void UISlider::processInput() {
		if (IsMouseOver(*this)) {
			if (Application::Input.isMouseButtonPressed(MOUSE_BUTTON_LEFT)
				&& !m_selected) {
				setSelected(true);
				m_parent->UIEvent(m_ID, SelectedEventID);
			}
		}
		if (!Application::Input.isMouseButtonDown(MOUSE_BUTTON_LEFT)
			&& m_selected && !m_highlighted) {
			setSelected(false);
			m_parent->UIEvent(m_ID, UnselectedEventID);
			m_parent->UIEvent(m_ID, ValueSetEventID);
		}
		if (m_selected && (IsMouseOver(*this) || !m_highlighted)) {
			glm::vec2 mp = Application::Input.getMouseCursorPosition(
				m_parent->getShader().getCamera());
			if (mp.x >= m_backgroundSprite.getPosition().x
				+ m_backgroundSprite.getDimensions().x
				- m_cursor.getDimensions().x) {
				m_value = 1.0f;
			}
			else if (mp.x < m_backgroundSprite.getPosition().x) {
				m_value = 0.0f;
			}
			else {
				m_value = (mp.x - m_backgroundSprite.getPosition().x)
					/ (m_backgroundSprite.getDimensions().x
						- m_cursor.getDimensions().x);
			}
		}
		if (m_highlighted) {
			setSelected(true);
			if (Application::Input.getControllerAxisPosition(
				m_parent->getController(),
				CONTROLLER_AXIS_LEFT_STICK_X) < -0.5f) {
				setValue(getValue() - 0.01f);
				m_parent->UIEvent(m_ID, ValueSetEventID);
			}
			else if (Application::Input.getControllerAxisPosition(
				m_parent->getController(),
				CONTROLLER_AXIS_LEFT_STICK_X) > 0.5f) {
				setValue(getValue() + 0.01f);
				m_parent->UIEvent(m_ID, ValueSetEventID);
			}
		}
	}

	void UISlider::update(float timeStep) {
		float cx = m_backgroundSprite.getPosition().x
			+ ((m_backgroundSprite.getDimensions().x
				- m_cursor.getDimensions().x) * m_value);
		float cy = 0.0f;
		if (m_verticalCursorAlignment == ALIGN_BOTTOM) {
			cy = m_backgroundSprite.getPosition().y;
		}
		else if (m_verticalCursorAlignment == ALIGN_CENTER) {
			cy = m_backgroundSprite.getPosition().y
				+ (m_backgroundSprite.getDimensions().y
					- m_cursor.getDimensions().y) / 2.0f;
		}
		else if (m_verticalCursorAlignment = ALIGN_TOP) {
			cy = m_backgroundSprite.getPosition().y
				+ m_backgroundSprite.getDimensions().y
				- m_cursor.getDimensions().y;
		}
		else {
			cy = m_backgroundSprite.getPosition().y
				+ (m_backgroundSprite.getDimensions().y
					- m_cursor.getDimensions().y) / 2.0f;
		}
		m_cursor.setPosition(glm::vec3(cx, cy,
			m_backgroundSprite.getPosition().z + 0.1f));
	}

	void UISlider::destroy() {
		UILabel::destroy();
		m_selected = false;
		m_value = 0.0f;
		m_cursor.destroy();
		m_verticalCursorAlignment = NO_ALIGNMENT;
	}

	// Define UITextBox class static variables

	glm::vec4 UITextBox::UnselectedTextureBox = glm::vec4(
		0.0f, 0.0f, 1.0f / 2.0f, 1.0f);
	glm::vec4 UITextBox::SelectedTextureBox = glm::vec4(
		1.0f / 2.0f, 0.0f, 1.0f / 2.0f, 1.0f);

	// Implement UITextBox class functions

	void UITextBox::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& labelText,
		const std::string& text, const std::string& allowedCharacters,
		int maximumCharacters, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment,
		Alignment verticalTextAlignment, Alignment verticalCursorAlignment) {
		UILabel::initialize(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		setSelected(false);
		setText(text);
		setAllowedCharacters(allowedCharacters);
		setMaximumCharacters(maximumCharacters);
		setVerticalTextAlignment(verticalTextAlignment);
		m_cursor.initialize(glm::vec3(), cursorDimensions,
			cursorTextureFileName);
		setCursorPosition(0);
		setVerticalCursorAlignment(verticalCursorAlignment);
	}

	bool UITextBox::isSelected() const {
		return m_selected;
	}

	void UITextBox::setSelected(bool selected) {
		if (!selected) {
			m_backgroundSprite.setTextureBox(UnselectedTextureBox);
		}
		else {
			m_backgroundSprite.setTextureBox(SelectedTextureBox);
		}
		m_selected = selected;
	}

	const std::string& UITextBox::getText() const {
		return m_text;
	}

	void UITextBox::setText(const std::string& text) {
		if (text.length() > m_maximumCharacters
			&& m_maximumCharacters > 0) {
			return;
		}
		m_horizontalTextPosition = m_backgroundSprite.getPosition().x;
		m_cursorPosition = 0;
		m_text = text;
	}

	const std::string& UITextBox::getAllowedCharacters() const {
		return m_allowedCharacters;
	}

	void UITextBox::setAllowedCharacters(const std::string& allowedCharacters) {
		m_allowedCharacters = allowedCharacters;
	}

	int UITextBox::getMaximumCharacters() const {
		return m_maximumCharacters;
	}

	void UITextBox::setMaximumCharacters(int maximumCharacters) {
		m_maximumCharacters = maximumCharacters;
		if (m_maximumCharacters >= 0 && m_maximumCharacters < m_text.length()) {
			m_text.resize(m_maximumCharacters);
		}
		setText(m_text);
	}

	Alignment UITextBox::getVerticalTextAlignment() const {
		return m_verticalTextAlignment;
	}

	void UITextBox::setVerticalTextAlignment(Alignment verticalTextAlignment) {
		m_verticalTextAlignment = verticalTextAlignment;
	}

	Sprite& UITextBox::getCursor() {
		return m_cursor;
	}

	void UITextBox::setCursor(const Sprite& cursor) {
		m_cursor = cursor;
	}

	unsigned int UITextBox::getCursorPosition() const {
		return m_cursorPosition;
	}

	void UITextBox::setCursorPosition(unsigned int cursorPosition) {
		if (cursorPosition > m_text.length()) {
			return;
		}
		m_cursorPosition = cursorPosition;
	}

	Alignment UITextBox::getVerticalCursorAlignment() const {
		return m_verticalCursorAlignment;
	}

	void UITextBox::setVerticalCursorAlignment(
		Alignment verticalCursorAlignment) {
		m_verticalCursorAlignment = verticalCursorAlignment;
	}

	void UITextBox::setPosition(const glm::vec3& position) {
		UILabel::setPosition(position);
		m_horizontalTextPosition = m_backgroundSprite.getPosition().x;
	}

	void UITextBox::setDimensions(const glm::vec2& dimensions) {
		glm::vec2 cdims = m_cursor.getDimensions();
		cdims.y *= (dimensions.y / getDimensions().y);
		m_cursor.setDimensions(cdims);
		UILabel::setDimensions(dimensions);
		m_horizontalTextPosition = m_backgroundSprite.getPosition().x;
	}

	void UITextBox::setEnabled(bool enabled) {
		UILabel::setEnabled(enabled);
		if (!m_enabled) {
			setSelected(false);
		}
	}

	void UITextBox::draw() {
		UILabel::draw();
		glm::vec3 textPosition(m_horizontalTextPosition,
			m_backgroundSprite.getPosition().y,
			m_backgroundSprite.getPosition().z + 0.1f);
		glm::vec4 textBox(m_backgroundSprite.getPosition().x,
			m_backgroundSprite.getPosition().y,
			m_backgroundSprite.getDimensions().x,
			m_backgroundSprite.getDimensions().y);
		Application::Graphics.submit(m_text, textPosition, textBox,
			m_parent->getTextScale(), m_parent->getTextColor(),
			m_parent->getFontFileName(), NO_ALIGNMENT, m_verticalTextAlignment,
			m_parent->getShader());
		if (m_selected && m_cursor.intersects(m_backgroundSprite)) {
			Application::Graphics.submit(m_cursor, m_parent->getShader());
		}
	}

	void UITextBox::processInput() {
		if (Application::Input.isMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (IsMouseOver(*this)) {
				if (!m_selected && Application::Input.isMouseButtonPressed(
					MOUSE_BUTTON_LEFT)) {
					setSelected(true);
					m_parent->UIEvent(m_ID, SelectedEventID);
				}
				float x = m_horizontalTextPosition;
				unsigned int cp = 0;
				bool s = false;
				const Font* font = Application::Graphics.getFont(
					m_parent->getFontFileName());
				glm::vec2 mp = Application::Input.getMouseCursorPosition(
					m_parent->getShader().getCamera());
				for (unsigned int i = 0; i < m_text.length(); i++) {
					float d = font->getGlyphs().at(m_text[i]).getOffset()
						* m_parent->getTextScale();
					if (x + d / 2.0f > mp.x) {
						s = true;
						cp = i;
						break;
					}
					x += d;
				}
				if (s) {
					m_cursorPosition = cp;
				}
				else {
					m_cursorPosition = (unsigned int)m_text.length();
				}
			}
			else {
				if (m_selected && Application::Input.isMouseButtonPressed(
					MOUSE_BUTTON_LEFT)) {
					setSelected(false);
					m_parent->UIEvent(m_ID, UnselectedEventID);
				}
			}
		}
		if (m_selected) {
			if (Application::Input.isKeyPressed(KEY_ENTER)) {
				m_parent->UIEvent(m_ID, TextEnteredEventID);
			}
			if (m_cursorPosition > 0
				&& (Application::Input.isKeyPressed(KEY_LEFT)
					|| (Application::Input.isKeyDown(KEY_LEFT) && m_timer
						> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				m_cursorPosition--;
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (Application::Input.isKeyPressed(KEY_RIGHT)
					|| (Application::Input.isKeyDown(KEY_RIGHT) && m_timer
			> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				m_cursorPosition++;
				m_timer = 0.0f;
			}
			if (m_cursorPosition > 0
				&& (Application::Input.isKeyPressed(KEY_BACKSPACE)
					|| (Application::Input.isKeyDown(KEY_BACKSPACE) && m_timer
						> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp + 1) {
						nt += m_text[cp];
					}
				}
				m_text = nt;
				m_cursorPosition--;
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (Application::Input.isKeyPressed(KEY_DELETE)
					|| (Application::Input.isKeyDown(KEY_DELETE) && m_timer
			> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp) {
						nt += m_text[cp];
					}
				}
				m_text = nt;
				m_timer = 0.0f;
			}
		}
		else if (m_highlighted) {
			setSelected(true);
			m_parent->UIEvent(m_ID, SelectedEventID);
		}
		if (!m_highlighted && m_lastHighlighted) {
			setSelected(false);
			m_parent->UIEvent(m_ID, UnselectedEventID);
		}
		m_lastHighlighted = m_highlighted;
		if (m_highlighted) {
			if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X) < -0.5f
				&& m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				setCursorPosition(getCursorPosition() - 1);
			}
			else if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X) > 0.5f
				&& m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				setCursorPosition(getCursorPosition() + 1);
			}
		}
	}

	void UITextBox::update(float timeStep) {
		glm::vec3 cursorPosition(m_horizontalTextPosition, 0.0f,
			m_backgroundSprite.getPosition().z + 0.2f);
		const Font* font = Application::Graphics.getFont(
			m_parent->getFontFileName());
		for (unsigned int cp = 0; cp < m_cursorPosition; cp++) {
			cursorPosition.x += (font->getGlyphs().at(m_text[cp]).getOffset()
				- (cp == 0
					? font->getGlyphs().at(m_text[cp]).getBearing().x : 0))
				* m_parent->getTextScale();
		}
		if (m_verticalCursorAlignment == ALIGN_BOTTOM) {
			cursorPosition.y = m_backgroundSprite.getPosition().y;
		}
		else if (m_verticalCursorAlignment == ALIGN_CENTER) {
			cursorPosition.y = m_backgroundSprite.getPosition().y
				+ (m_backgroundSprite.getDimensions().y
					- m_cursor.getDimensions().y) / 2.0f;
		}
		else if (m_verticalCursorAlignment == ALIGN_TOP) {
			cursorPosition.y = m_backgroundSprite.getPosition().y
				+ m_backgroundSprite.getDimensions().y
				- m_cursor.getDimensions().y;
		}
		else {
			cursorPosition.y = m_backgroundSprite.getPosition().y
				+ (m_backgroundSprite.getDimensions().y
					- m_cursor.getDimensions().y) / 2.0f;
		}
		m_cursor.setPosition(cursorPosition);
		if (m_cursor.getPosition().x < m_backgroundSprite.getPosition().x) {
			m_horizontalTextPosition
				+= m_backgroundSprite.getDimensions().x / 5.0f;
			if (m_horizontalTextPosition > m_backgroundSprite.getPosition().x) {
				m_horizontalTextPosition = m_backgroundSprite.getPosition().x;
			}
		}
		else if (m_cursor.getPosition().x + m_cursor.getDimensions().x
		> m_backgroundSprite.getPosition().x
			+ m_backgroundSprite.getDimensions().x) {
			m_horizontalTextPosition
				-= m_backgroundSprite.getDimensions().x / 5.0f;
		}
		m_timer += timeStep;
		if (m_timer > (float)Application::Window.getTargetUpdatesPerSecond()) {
			m_timer = 0.0f;
		}
	}

	void UITextBox::destroy() {
		UILabel::destroy();
		m_selected = false;
		m_text = "";
		m_allowedCharacters = "";
		m_maximumCharacters = 0;
		m_horizontalTextPosition = 0.0f;
		m_verticalTextAlignment = NO_ALIGNMENT;
		m_cursor.destroy();
		m_cursorPosition = 0;
		m_verticalCursorAlignment = NO_ALIGNMENT;
		m_timer = 0.0f;
	}

	void UITextBox::characterTyped(char character) {
		if (!m_selected) {
			return;
		}
		if (m_text.length() >= m_maximumCharacters && m_maximumCharacters > 0) {
			m_text.resize(m_maximumCharacters);
			return;
		}
		bool f = false;
		for (char x : m_allowedCharacters) {
			if (x == character) {
				f = true;
				break;
			}
		}
		if (!(m_allowedCharacters.empty() || f)) {
			return;
		}
		std::string nt = "";
		bool s = false;
		for (unsigned int cp = 0; cp < m_text.length(); cp++) {
			if (cp == m_cursorPosition) {
				nt += character;
				s = true;
			}
			nt += m_text[cp];
		}
		if (s) {
			m_text = nt;
		}
		else {
			m_text += character;
		}
		m_cursorPosition++;
	}

	// Implement UIPasswordBox class functions

	void UIPasswordBox::draw() {
		UILabel::draw();
		glm::vec3 textPosition(m_horizontalTextPosition,
			m_backgroundSprite.getPosition().y,
			m_backgroundSprite.getPosition().z + 0.1f);
		glm::vec4 textBox(m_backgroundSprite.getPosition().x,
			m_backgroundSprite.getPosition().y,
			m_backgroundSprite.getDimensions().x,
			m_backgroundSprite.getDimensions().y);
		std::string maskText = "";
		for (size_t i = 0; i < m_text.length(); i++) {
			maskText += "*";
		}
		Application::Graphics.submit(maskText, textPosition, textBox,
			m_parent->getTextScale(), m_parent->getTextColor(),
			m_parent->getFontFileName(), NO_ALIGNMENT, m_verticalTextAlignment,
			m_parent->getShader());
		if (m_selected) {
			Application::Graphics.submit(m_cursor, m_parent->getShader());
		}
	}

	void UIPasswordBox::processInput() {
		if (Application::Input.isMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (IsMouseOver(*this)) {
				if (!m_selected && Application::Input.isMouseButtonPressed(
					MOUSE_BUTTON_LEFT)) {
					setSelected(true);
					m_parent->UIEvent(m_ID, SelectedEventID);
				}
				float x = m_horizontalTextPosition;
				unsigned int cp = 0;
				bool s = false;
				const Font* font = Application::Graphics.getFont(
					m_parent->getFontFileName());
				glm::vec2 mp = Application::Input.getMouseCursorPosition(
					m_parent->getShader().getCamera());
				for (unsigned int i = 0; i < m_text.length(); i++) {
					float d = font->getGlyphs().at('*').getOffset()
						* m_parent->getTextScale();
					if (x + d / 2.0f > mp.x) {
						s = true;
						cp = i;
						break;
					}
					x += d;
				}
				if (s) {
					m_cursorPosition = cp;
				}
				else {
					m_cursorPosition = (unsigned int)m_text.length();
				}
			}
			else {
				if (m_selected && Application::Input.isMouseButtonPressed(
					MOUSE_BUTTON_LEFT)) {
					setSelected(false);
					m_parent->UIEvent(m_ID, UnselectedEventID);
				}
			}
		}
		if (m_selected) {
			if (Application::Input.isKeyPressed(KEY_ENTER)) {
				m_parent->UIEvent(m_ID, TextEnteredEventID);
			}
			if (m_cursorPosition > 0
				&& (Application::Input.isKeyPressed(KEY_LEFT)
					|| (Application::Input.isKeyDown(KEY_LEFT) && m_timer
		> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				m_cursorPosition--;
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (Application::Input.isKeyPressed(KEY_RIGHT)
					|| (Application::Input.isKeyDown(KEY_RIGHT) && m_timer
			> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				m_cursorPosition++;
				m_timer = 0.0f;
			}
			if (m_cursorPosition > 0
				&& (Application::Input.isKeyPressed(KEY_BACKSPACE)
					|| (Application::Input.isKeyDown(KEY_BACKSPACE) && m_timer
						> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp + 1) {
						nt += m_text[cp];
					}
				}
				m_text = nt;
				m_cursorPosition--;
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (Application::Input.isKeyPressed(KEY_DELETE)
					|| (Application::Input.isKeyDown(KEY_DELETE) && m_timer
			> (float)Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp) {
						nt += m_text[cp];
					}
				}
				m_text = nt;
				m_timer = 0.0f;
			}
		}
		else if (m_highlighted) {
			setSelected(true);
			m_parent->UIEvent(m_ID, SelectedEventID);
		}
		if (!m_highlighted && m_lastHighlighted) {
			setSelected(false);
			m_parent->UIEvent(m_ID, UnselectedEventID);
		}
		m_lastHighlighted = m_highlighted;
		if (m_highlighted) {
			if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X) < -0.5f
				&& m_timer > (float)Application::Window
				.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				setCursorPosition(getCursorPosition() - 1);
			}
			else if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X) > 0.5f
				&& m_timer > (float)Application::Window
				.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				setCursorPosition(getCursorPosition() + 1);
			}
		}
	}

	void UIPasswordBox::update(float timeStep) {
		glm::vec3 cursorPosition(m_horizontalTextPosition, 0.0f,
			m_backgroundSprite.getPosition().z + 0.2f);
		const Font* font = Application::Graphics.getFont(
			m_parent->getFontFileName());
		for (unsigned int cp = 0; cp < m_cursorPosition; cp++) {
			cursorPosition.x += (font->getGlyphs().at('*').getOffset()
				- (cp == 0 ? font->getGlyphs().at('*').getBearing().x : 0))
				* m_parent->getTextScale();
		}
		if (m_verticalCursorAlignment == ALIGN_BOTTOM) {
			cursorPosition.y = m_backgroundSprite.getPosition().y;
		}
		else if (m_verticalCursorAlignment == ALIGN_CENTER) {
			cursorPosition.y = m_backgroundSprite.getPosition().y
				+ (m_backgroundSprite.getDimensions().y
					- m_cursor.getDimensions().y) / 2.0f;
		}
		else if (m_verticalCursorAlignment == ALIGN_TOP) {
			cursorPosition.y = m_backgroundSprite.getPosition().y
				+ m_backgroundSprite.getDimensions().y
				- m_cursor.getDimensions().y;
		}
		else {
			cursorPosition.y = m_backgroundSprite.getPosition().y
				+ (m_backgroundSprite.getDimensions().y
					- m_cursor.getDimensions().y) / 2.0f;
		}
		m_cursor.setPosition(cursorPosition);
		if (m_cursor.getPosition().x < m_backgroundSprite.getPosition().x) {
			m_horizontalTextPosition
				+= m_backgroundSprite.getDimensions().x / 5.0f;
			if (m_horizontalTextPosition > m_backgroundSprite.getPosition().x) {
				m_horizontalTextPosition = m_backgroundSprite.getPosition().x;
			}
		}
		else if (m_cursor.getPosition().x + m_cursor.getDimensions().x
		> m_backgroundSprite.getPosition().x
			+ m_backgroundSprite.getDimensions().x) {
			m_horizontalTextPosition
				-= m_backgroundSprite.getDimensions().x / 5.0f;
		}
		m_timer += timeStep;
		if (m_timer > (float)Application::Window.getTargetUpdatesPerSecond()) {
			m_timer = 0.0f;
		}
	}

	void UIPasswordBox::destroy() {
		for (size_t i = 0; i < m_text.length(); i++) {
			m_text[i] = 0x00;
		}
		UITextBox::destroy();
	}

	// Implement UITextArea class functions

	void UITextArea::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, unsigned int lineCount,
		const std::string& textureFileName, const glm::vec2& cursorDimensions,
		const std::string& cursorTextureFileName, const std::string& labelText,
		const std::string& text, const std::string& allowedCharacters,
		int maximumCharacters, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment,
		Alignment verticalCursorAlignment) {
		UITextBox::initialize(position, dimensions, textureFileName,
			cursorDimensions, cursorTextureFileName, labelText, text,
			allowedCharacters, maximumCharacters, labelAlignment,
			horizontalLabelAlignment, verticalLabelAlignment, ALIGN_CENTER,
			verticalCursorAlignment);
		setLineCount(lineCount);
		setTopLineIndex(0);
	}

	void UITextArea::setPosition(const glm::vec3& position) {
		UITextBox::setPosition(position);
		m_topLineIndex = 0;
	}

	void UITextArea::setDimensions(const glm::vec2& dimensions) {
		UITextBox::setDimensions(dimensions);
		m_topLineIndex = 0;
	}

	void UITextArea::setText(const std::string& text) {
		UITextBox::setText(text);
		m_topLineIndex = 0;
	}

	unsigned int UITextArea::getLineCount() const {
		return m_lineCount;
	}

	void UITextArea::setLineCount(unsigned int lineCount) {
		m_lineCount = lineCount;
	}

	int UITextArea::getTopLineIndex() const {
		return m_topLineIndex;
	}

	void UITextArea::setTopLineIndex(int topLineIndex) {
		if (topLineIndex < 0) {
			return;
		}
		int contentLineCount = (int)getContentLines().size();
		if (contentLineCount <= (int)m_lineCount && topLineIndex > 0) {
			return;
		}
		if (topLineIndex > contentLineCount - (int)m_lineCount) {
			return;
		}
		m_topLineIndex = topLineIndex;
	}

	void UITextArea::draw() {
		UILabel::draw();
		std::vector<std::string> contentLines = getContentLines();
		float lineHeight = m_backgroundSprite.getDimensions().y
			/ (float)m_lineCount;
		for (size_t i = 0; i < m_lineCount; i++) {
			glm::vec3 linePos(m_horizontalTextPosition,
				(m_backgroundSprite.getPosition().y
					+ m_backgroundSprite.getDimensions().y)
					- (lineHeight * (i + 1)),
				m_backgroundSprite.getPosition().z + 0.1f);
			glm::vec4 lineBox(m_backgroundSprite.getPosition().x, linePos.y,
				m_backgroundSprite.getDimensions().x, lineHeight);
			if (i + m_topLineIndex >= contentLines.size()) {
				break;
			}
			Application::Graphics.submit(contentLines[i + m_topLineIndex],
				linePos, lineBox, m_parent->getTextScale(),
				m_parent->getTextColor(), m_parent->getFontFileName(),
				NO_ALIGNMENT, ALIGN_CENTER, m_parent->getShader());
		}
		if (m_selected && m_cursor.intersects(m_backgroundSprite)) {
			Application::Graphics.submit(m_cursor, m_parent->getShader());
		}
	}

	void UITextArea::processInput() {
		if (Application::Input.isMouseButtonDown(MOUSE_BUTTON_LEFT)) {
			if (IsMouseOver(*this)) {
				if (!m_selected && Application::Input.isMouseButtonPressed(
					MOUSE_BUTTON_LEFT)) {
					setSelected(true);
					m_parent->UIEvent(m_ID, SelectedEventID);
				}
				float x = m_horizontalTextPosition;
				float y = (m_backgroundSprite.getPosition().y
						+ m_backgroundSprite.getDimensions().y)
					- (m_backgroundSprite.getDimensions().y / m_lineCount);
				const Font* font = Application::Graphics.getFont(
					m_parent->getFontFileName());
				glm::vec2 mp = Application::Input.getMouseCursorPosition(
					m_parent->getShader().getCamera());
				m_cursorPosition = 0;
				std::vector<std::string> contentLines = getContentLines();
				if (!contentLines.empty()) {
					unsigned int line = 0;
					for (unsigned int l = 0; l < contentLines.size(); l++) {
						float yOffset = (m_backgroundSprite.getDimensions().y
							/ m_lineCount) * m_topLineIndex;
						if (y <= mp.y - yOffset) {
							line = l;
							break;
						}
						y -= (m_backgroundSprite.getDimensions().y
							/ m_lineCount);
						m_cursorPosition
							+= (unsigned int)contentLines[l].length() + 1;
					}
					float xd = 0.0f;
					for (unsigned int i = 0; i < contentLines[line].length();
						i++) {
						xd = font->getGlyphs().at(contentLines[line][i])
							.getOffset() * m_parent->getTextScale();
						if (x + xd / 2.0f > mp.x) {
							break;
						}
						m_cursorPosition++;
						x += xd;
					}
					if (m_cursorPosition > m_text.length()) {
						m_cursorPosition = (unsigned int)m_text.length();
					}
				}
			}
			else {
				if (m_selected && Application::Input.isMouseButtonPressed(
					MOUSE_BUTTON_LEFT)) {
					setSelected(false);
					m_parent->UIEvent(m_ID, UnselectedEventID);
				}
			}
		}
		if (m_selected) {
			if (Application::Input.isKeyPressed(KEY_ENTER)) {
				characterTyped('\n');
			}
			if (m_cursorPosition > 0
				&& (Application::Input.isKeyPressed(KEY_LEFT)
					|| (Application::Input.isKeyDown(KEY_LEFT) && m_timer
						> Application::Window.getTargetUpdatesPerSecond()
							/ 10.0f))) {
				m_cursorPosition--;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (Application::Input.isKeyPressed(KEY_RIGHT)
					|| (Application::Input.isKeyDown(KEY_RIGHT) && m_timer
						> Application::Window.getTargetUpdatesPerSecond()
							/ 10.0f))) {
				m_cursorPosition++;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (Application::Input.isKeyPressed(KEY_UP)
				|| (Application::Input.isKeyDown(KEY_UP) && m_timer
					> Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f)) {
				moveCursorUp();
				m_timer = 0.0f;
			}
			if (Application::Input.isKeyPressed(KEY_DOWN)
				|| (Application::Input.isKeyDown(KEY_DOWN) && m_timer
					> Application::Window.getTargetUpdatesPerSecond()
						/ 6.0f)) {
				moveCursorDown();
				m_timer = 0.0f;
			}
			if (m_cursorPosition > 0
				&& (Application::Input.isKeyPressed(KEY_BACKSPACE)
					|| (Application::Input.isKeyDown(KEY_BACKSPACE) && m_timer
						> Application::Window.getTargetUpdatesPerSecond()
							/ 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp + 1) {
						nt += m_text[cp];
					}
				}
				m_text = nt;
				m_cursorPosition--;
				putCursorInView();
				m_timer = 0.0f;
			}
			if (m_cursorPosition < m_text.length()
				&& (Application::Input.isKeyPressed(KEY_DELETE)
					|| (Application::Input.isKeyDown(KEY_DELETE) && m_timer
						> Application::Window.getTargetUpdatesPerSecond()
							/ 10.0f))) {
				std::string nt = "";
				for (unsigned int cp = 0; cp < m_text.length(); cp++) {
					if (m_cursorPosition != cp) {
						nt += m_text[cp];
					}
				}
				m_text = nt;
				putCursorInView();
				m_timer = 0.0f;
			}
		}
		else if (m_highlighted) {
			setSelected(true);
			m_parent->UIEvent(m_ID, SelectedEventID);
		}
		if (!m_highlighted && m_lastHighlighted) {
			setSelected(false);
			m_parent->UIEvent(m_ID, UnselectedEventID);
		}
		m_lastHighlighted = m_highlighted;
		if (IsMouseOver(*this)) {
			float scroll = Application::Input.getMouseScrollMovement().y;
			if (scroll != 0.0f && m_timer
				> Application::Window.getTargetUpdatesPerSecond() / 10.0f) {
				if (scroll < 0.0f) {
					setTopLineIndex(getTopLineIndex() + 1);
				}
				else {
					setTopLineIndex(getTopLineIndex() - 1);
				}
				m_timer = 0.0f;
			}
		}
		if (m_highlighted) {
			if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X) < -0.5f
				&& m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				setCursorPosition(getCursorPosition() - 1);
			}
			else if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X) > 0.5f
				&& m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				setCursorPosition(getCursorPosition() + 1);
			}
			if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_Y) < -0.5f
				&& m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				moveCursorUp();
			}
			else if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_Y) > 0.5f
				&& m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
				m_timer = 0.0f;
				moveCursorDown();
			}
		}
	}

	void UITextArea::update(float timeStep) {
		m_cursor.setPosition(glm::vec3(m_horizontalTextPosition,
			m_backgroundSprite.getPosition().y
			+ m_backgroundSprite.getDimensions().y + ((m_topLineIndex - 1)
				* (m_backgroundSprite.getDimensions().y / m_lineCount)),
			m_backgroundSprite.getPosition().z + 0.2f));
		const Font* font = Application::Graphics.getFont(
			m_parent->getFontFileName());
		for (unsigned int cp = 0; cp < m_cursorPosition; cp++) {
			if (m_text[cp] == '\n') {
				glm::vec3 cpos = m_cursor.getPosition();
				m_cursor.setPosition(glm::vec3(m_horizontalTextPosition,
					cpos.y - (m_backgroundSprite.getDimensions().y
						/ m_lineCount), cpos.z));
			}
			else {
				glm::vec3 cpos = m_cursor.getPosition();
				cpos.x += (font->getGlyphs().at(m_text[cp]).getOffset()
					- (cp == 0 ? font->getGlyphs().at(m_text[cp]).getBearing().x
						: 0)) * m_parent->getTextScale();
				m_cursor.setPosition(cpos);
			}
		}
		if (m_verticalCursorAlignment == ALIGN_CENTER) {
			glm::vec3 cpos = m_cursor.getPosition();
			cpos.y = cpos.y
				+ ((m_backgroundSprite.getDimensions().y / m_lineCount)
					- m_cursor.getDimensions().y) / 2.0f;
			m_cursor.setPosition(cpos);
		}
		else if (m_verticalCursorAlignment == ALIGN_TOP) {
			glm::vec3 cpos = m_cursor.getPosition();
			cpos.y = cpos.y
				+ (m_backgroundSprite.getDimensions().y / m_lineCount)
				- m_cursor.getDimensions().y;
			m_cursor.setPosition(cpos);
		}
		m_timer += timeStep;
		if (m_timer > (float)Application::Window.getTargetUpdatesPerSecond()) {
			m_timer = 0.0f;
		}
	}

	void UITextArea::destroy() {
		UITextBox::destroy();
		m_lineCount = 0;
		m_topLineIndex = 0;
	}

	void UITextArea::characterTyped(char character) {
		UITextBox::characterTyped(character);
		putCursorInView();
	}

	void UITextArea::moveCursorDown() {
		if (m_text.empty()) {
			return;
		}
		std::vector<std::string> contentLines = getContentLines();
		unsigned int cLine = 0;
		for (unsigned int i = 0; i < m_cursorPosition; i++) {
			if (m_text[i] == '\n') {
				cLine++;
			}
		}
		if (cLine < contentLines.size() - 1) {
			unsigned int ncp = 0;
			for (unsigned int i = 0; i < cLine + 1; i++) {
				ncp += (unsigned int)contentLines[i].length() + 1;
			}
			float x = m_horizontalTextPosition;
			const Font* font = Application::Graphics.getFont(
				m_parent->getFontFileName());
			for (unsigned int i = 0;
				i < contentLines[cLine + 1].length(); i++) {
				x += font->getGlyphs().at(contentLines[cLine + 1][i])
					.getOffset() * m_parent->getTextScale();
				ncp++;
				if (x + font->getGlyphs().at(contentLines[cLine + 1][i])
					.getOffset() * m_parent->getTextScale()
				> m_cursor.getPosition().x) {
					break;
				}
			}
			m_cursorPosition = ncp;
		}
		putCursorInView();
	}

	void UITextArea::moveCursorUp() {
		if (m_text.empty()) {
			return;
		}
		std::vector<std::string> contentLines = getContentLines();
		unsigned int cLine = 0;
		for (unsigned int i = 0; i < m_cursorPosition; i++) {
			if (m_text[i] == '\n') {
				cLine++;
			}
		}
		if (cLine > 0) {
			unsigned int ncp = 0;
			for (unsigned int i = 0; i < cLine - 1; i++) {
				ncp += (unsigned int)contentLines[i].length() + 1;
			}
			float x = m_horizontalTextPosition;
			const Font* font = Application::Graphics.getFont(
				m_parent->getFontFileName());
			for (unsigned int i = 0;
				i < contentLines[cLine - 1].length(); i++) {
				x += font->getGlyphs().at(contentLines[cLine - 1][i])
					.getOffset() * m_parent->getTextScale();
				ncp++;
				if (x + font->getGlyphs().at(
					contentLines[cLine - 1][i]).getOffset()
					* m_parent->getTextScale()
				> m_cursor.getPosition().x) {
					break;
				}
			}
			m_cursorPosition = ncp;
		}
		putCursorInView();
	}

	void UITextArea::putCursorInView() {
		update(0.0f);
		bool goodX = false;
		while (!goodX) {
			if (m_cursor.getPosition().x < m_backgroundSprite.getPosition().x) {
				m_horizontalTextPosition
					+= m_backgroundSprite.getDimensions().x / 5.0f;
				glm::vec3 cp = m_cursor.getPosition();
				cp.x += m_backgroundSprite.getDimensions().x / 5.0f;
				m_cursor.setPosition(cp);
				if (m_horizontalTextPosition
					> m_backgroundSprite.getPosition().x) {
					m_horizontalTextPosition
						= m_backgroundSprite.getPosition().x;
					goodX = true;
				}
			}
			else if (m_cursor.getPosition().x + m_cursor.getDimensions().x
				> m_backgroundSprite.getPosition().x
					+ m_backgroundSprite.getDimensions().x) {
				m_horizontalTextPosition
					-= m_backgroundSprite.getDimensions().x / 5.0f;
				glm::vec3 cp = m_cursor.getPosition();
				cp.x -= m_backgroundSprite.getDimensions().x / 5.0f;
				m_cursor.setPosition(cp);
			}
			else {
				goodX = true;
			}
		}
		bool goodY = false;
		while (!goodY) {
			if (m_cursor.getPosition().y < m_backgroundSprite.getPosition().y) {
				setTopLineIndex(m_topLineIndex + 1);
				glm::vec3 cp = m_cursor.getPosition();
				cp.y += (m_backgroundSprite.getDimensions().y
					/ (float)m_lineCount);
				m_cursor.setPosition(cp);
			}
			else if (m_cursor.getPosition().y + m_cursor.getDimensions().y
				> m_backgroundSprite.getPosition().y
					+ m_backgroundSprite.getDimensions().y) {
				setTopLineIndex(m_topLineIndex - 1);
				glm::vec3 cp = m_cursor.getPosition();
				cp.y -= (m_backgroundSprite.getDimensions().y
					/ (float)m_lineCount);
				m_cursor.setPosition(cp);
			}
			else {
				goodY = true;
			}
		}
	}

	std::vector<std::string> UITextArea::getContentLines() const {
		if (m_text.empty()) {
			return std::vector<std::string>();
		}
		std::vector<std::string> contentLines;
		std::string contentLine = "";
		for (size_t i = 0; i < m_text.length(); i++) {
			if (m_text[i] == '\n') {
				contentLines.push_back(contentLine);
				contentLine = "";
			}
			else {
				contentLine += m_text[i];
			}
		}
		if (!contentLine.empty()) {
			contentLines.push_back(contentLine);
		}
		return contentLines;
	}

	// Implement UICarousel class functions

	void UICarousel::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const std::string& labelText, const std::vector<std::string>& values,
		const std::string& initialValue, float buttonWidth,
		const std::string& buttonTextureFileName, Alignment labelAlignment,
		Alignment horizontalLabelAlignment, Alignment verticalLabelAlignment,
		Alignment horizontalTextAlignment, Alignment verticalTextAlignment,
		Alignment horizontalButtonAlignment) {
		m_horizontalButtonAlignment = horizontalButtonAlignment;
		UILabel::initialize(glm::vec3(),
			glm::vec2(horizontalButtonAlignment == ALIGN_CENTER
				? dimensions.x * (1.0f - (buttonWidth * 2.0f))
				: dimensions.x * (1.0f - buttonWidth), dimensions.y),
			textureFileName, labelText, labelAlignment,
			horizontalLabelAlignment, verticalLabelAlignment);
		for (const std::string& value : values) {
			addValue(value);
		}
		setCurrentValue(initialValue);
		m_lastButton.initialize(glm::vec3(),
			glm::vec2(dimensions.x * buttonWidth,
				horizontalButtonAlignment == ALIGN_CENTER
				? dimensions.y : dimensions.y / 2.0f), buttonTextureFileName,
			"", NO_ALIGNMENT, NO_ALIGNMENT, NO_ALIGNMENT);
		m_lastButton.setID(0);
		m_lastButton.setParent(*this);
		m_nextButton.initialize(glm::vec3(),
			glm::vec2(dimensions.x * buttonWidth,
				horizontalButtonAlignment == ALIGN_CENTER
				? dimensions.y : dimensions.y / 2.0f), buttonTextureFileName,
			"", NO_ALIGNMENT, NO_ALIGNMENT, NO_ALIGNMENT);
		m_nextButton.setID(1);
		m_nextButton.setParent(*this);
		setHorizontalTextAlignment(horizontalTextAlignment);
		setVerticalTextAlignment(verticalTextAlignment);
		setDimensions(dimensions);
		setPosition(position);
		setHorizontalButtonAlignment(horizontalButtonAlignment);
	}

	const std::vector<std::string>& UICarousel::getValues() const {
		return m_values;
	}

	const std::string UICarousel::getValue(unsigned int index) const {
		if (index >= m_values.size()) {
			return "";
		}
		return m_values.at(index);
	}

	int UICarousel::getValueIndex(const std::string& value) const {
		for (unsigned int index = 0; index < m_values.size(); index++) {
			if (m_values[index] == value) {
				return index;
			}
		}
		return -1;
	}

	bool UICarousel::addValue(const std::string& value, int index) {
		if (std::find(m_values.begin(), m_values.end(), value)
			!= m_values.end()) {
			return false;
		}
		if (index > (int)m_values.size()) {
			return false;
		}
		if (index < 0) {
			m_values.push_back(value);
			return true;
		}
		std::vector<std::string>::iterator it = m_values.begin() + index;
		m_values.insert(it, value);
		return true;
	}

	bool UICarousel::removeValue(const std::string& value) {
		std::vector<std::string>::iterator it = std::find(m_values.begin(),
			m_values.end(), value);
		if (it == m_values.end()) {
			return false;
		}
		m_values.erase(it);
		setCurrentValueIndex(m_currentValueIndex);
		return true;
	}

	bool UICarousel::removeValue(unsigned int index) {
		if (index >= m_values.size()) {
			return false;
		}
		std::vector<std::string>::iterator it = m_values.begin() + index;
		m_values.erase(it);
		setCurrentValueIndex(m_currentValueIndex);
		return true;
	}

	void UICarousel::clear() {
		m_values.clear();
		m_currentValueIndex = 0;
	}

	const std::string& UICarousel::getCurrentValue() const {
		return m_values.at(m_currentValueIndex);
	}

	void UICarousel::setCurrentValue(const std::string& value) {
		int index = getValueIndex(value);
		setCurrentValueIndex(index);
	}

	unsigned int UICarousel::getCurrentValueIndex() const {
		return m_currentValueIndex;
	}

	void UICarousel::setCurrentValueIndex(unsigned int index) {
		if (m_values.empty()) {
			return;
		}
		if (index >= m_values.size()) {
			m_currentValueIndex = (unsigned int)m_values.size() - 1;
		}
		else {
			m_currentValueIndex = index;
		}
	}

	UIButton& UICarousel::getLastButton() {
		return m_lastButton;
	}

	void UICarousel::setLastButton(const UIButton& lastButton) {
		m_lastButton = lastButton;
	}

	UIButton& UICarousel::getNextButton() {
		return m_nextButton;
	}

	void UICarousel::setNextButton(const UIButton& nextButton) {
		m_nextButton = nextButton;
	}

	Alignment UICarousel::getHorizontalTextAlignment() const {
		return m_horizontalTextAlignment;
	}

	void UICarousel::setHorizontalTextAlignment(
		Alignment horizontalTextAlignment) {
		m_horizontalTextAlignment = horizontalTextAlignment;
	}

	Alignment UICarousel::getVerticalTextAlignment() const {
		return m_verticalTextAlignment;
	}

	void UICarousel::setVerticalTextAlignment(
		Alignment verticalTextAlignment) {
		m_verticalTextAlignment = verticalTextAlignment;
	}

	Alignment UICarousel::getHorizontalButtonAlignment() const {
		return m_horizontalButtonAlignment;
	}

	void UICarousel::setHorizontalButtonAlignment(
		Alignment horizontalButtonAlignment) {
		glm::vec3 pos = getPosition();
		glm::vec2 dims = getDimensions();
		if (horizontalButtonAlignment == ALIGN_LEFT) {
			m_lastButton.getBackgroundSprite().setReflectedHorizontally(false);
			m_lastButton.getBackgroundSprite().setReflectedVertically(false);
			m_nextButton.getBackgroundSprite().setReflectedHorizontally(false);
			m_nextButton.getBackgroundSprite().setReflectedVertically(true);
		}
		else if (horizontalButtonAlignment == ALIGN_CENTER) {
			m_lastButton.getBackgroundSprite().setReflectedHorizontally(false);
			m_lastButton.getBackgroundSprite().setReflectedVertically(false);
			m_nextButton.getBackgroundSprite().setReflectedHorizontally(true);
			m_nextButton.getBackgroundSprite().setReflectedVertically(false);
		}
		else if (horizontalButtonAlignment == ALIGN_RIGHT) {
			m_lastButton.getBackgroundSprite().setReflectedHorizontally(false);
			m_lastButton.getBackgroundSprite().setReflectedVertically(false);
			m_nextButton.getBackgroundSprite().setReflectedHorizontally(false);
			m_nextButton.getBackgroundSprite().setReflectedVertically(true);
		}
		else {
			return;
		}
		m_horizontalButtonAlignment = horizontalButtonAlignment;
		setDimensions(dims);
		setPosition(pos);
	}

	glm::vec3 UICarousel::getPosition() const {
		if (m_horizontalButtonAlignment == ALIGN_LEFT) {
			return m_nextButton.getPosition();
		}
		else if (m_horizontalButtonAlignment == ALIGN_CENTER) {
			return m_lastButton.getPosition();
		}
		else if (m_horizontalButtonAlignment == ALIGN_RIGHT) {
			return UILabel::getPosition();
		}
		else {
			return glm::vec3();
		}
	}

	void UICarousel::setPosition(const glm::vec3& position) {
		if (m_horizontalButtonAlignment == ALIGN_LEFT) {
			m_nextButton.setPosition(position);
			m_lastButton.setPosition(glm::vec3(position.x,
				position.y + m_nextButton.getDimensions().y, position.z));
			UILabel::setPosition(
				glm::vec3(position.x + m_nextButton.getDimensions().x,
					position.y, position.z));
		}
		else if (m_horizontalButtonAlignment == ALIGN_CENTER) {
			m_lastButton.setPosition(position);
			UILabel::setPosition(glm::vec3(
				position.x + m_lastButton.getDimensions().x, position.y,
				position.z));
			m_nextButton.setPosition(glm::vec3(position.x
				+ m_lastButton.getDimensions().x + UILabel::getDimensions().x,
				position.y, position.z));
		}
		else if (m_horizontalButtonAlignment == ALIGN_RIGHT) {
			UILabel::setPosition(position);
			m_lastButton.setPosition(glm::vec3(
				position.x + UILabel::getDimensions().x,
				position.y + m_nextButton.getDimensions().y, position.z));
			m_nextButton.setPosition(glm::vec3(
				position.x + UILabel::getDimensions().x, position.y,
				position.z));
		}
	}

	glm::vec2 UICarousel::getDimensions() const {
		if (m_horizontalButtonAlignment == ALIGN_LEFT) {
			return glm::vec2(
				m_nextButton.getDimensions().x + UILabel::getDimensions().x,
				UILabel::getDimensions().y);
		}
		else if (m_horizontalButtonAlignment == ALIGN_CENTER) {
			return glm::vec2(m_lastButton.getDimensions().x
				+ UILabel::getDimensions().x + m_nextButton.getDimensions().x,
				UILabel::getDimensions().y);
		}
		else if (m_horizontalButtonAlignment == ALIGN_RIGHT) {
			return glm::vec2(
				UILabel::getDimensions().x + m_nextButton.getDimensions().x,
				UILabel::getDimensions().y);
		}
		return glm::vec2();
	}

	void UICarousel::setDimensions(const glm::vec2& dimensions) {
		float buttonWidth = m_nextButton.getDimensions().x
			/ getDimensions().x;
		if (m_horizontalButtonAlignment == ALIGN_LEFT) {
			m_lastButton.setDimensions(glm::vec2(dimensions.x * buttonWidth,
				dimensions.y / 2.0f));
			m_nextButton.setDimensions(glm::vec2(dimensions.x * buttonWidth,
				dimensions.y / 2.0f));
			UILabel::setDimensions(glm::vec2(
				dimensions.x - (dimensions.x * buttonWidth), dimensions.y));
		}
		else if (m_horizontalButtonAlignment == ALIGN_CENTER) {
			m_lastButton.setDimensions(glm::vec2(dimensions.x * buttonWidth,
				dimensions.y));
			UILabel::setDimensions(glm::vec2(
				dimensions.x - (2 * dimensions.x * buttonWidth), dimensions.y));
			m_nextButton.setDimensions(glm::vec2(dimensions.x * buttonWidth,
				dimensions.y));
		}
		else if (m_horizontalButtonAlignment == ALIGN_RIGHT) {
			m_lastButton.setDimensions(glm::vec2(dimensions.x * buttonWidth,
				dimensions.y / 2.0f));
			m_nextButton.setDimensions(glm::vec2(dimensions.x * buttonWidth,
				dimensions.y / 2.0f));
			UILabel::setDimensions(glm::vec2(
				dimensions.x - (dimensions.x * buttonWidth), dimensions.y));
		}
		setPosition(getPosition());
	}

	void UICarousel::setEnabled(bool enabled) {
		UILabel::setEnabled(enabled);
		m_lastButton.setEnabled(enabled);
		m_nextButton.setEnabled(enabled);
	}

	GLSLShader& UICarousel::getShader() {
		return m_parent->getShader();
	}

	void UICarousel::setShader(const GLSLShader&) {

	}

	const std::string& UICarousel::getFontFileName() const {
		return m_parent->getFontFileName();
	}

	void UICarousel::setFontFileName(const std::string&) {

	}

	float UICarousel::getTextScale() const {
		return m_parent->getTextScale();
	}

	void UICarousel::setTextScale(float) {

	}

	const glm::vec4& UICarousel::getTextColor() const {
		return m_parent->getTextColor();
	}

	void UICarousel::setTextColor(const glm::vec4&) {

	}

	Controller UICarousel::getController() const {
		return m_parent->getController();
	}

	void UICarousel::setController(Controller) {

	}

	void UICarousel::draw() {
		UILabel::draw();
		if (m_values.size() > 0) {
			Application::Graphics.submit(m_values.at(m_currentValueIndex),
				glm::vec3(m_backgroundSprite.getPosition().x,
					m_backgroundSprite.getPosition().y,
					m_backgroundSprite.getPosition().z + 0.1f),
				glm::vec4(m_backgroundSprite.getPosition().x,
					m_backgroundSprite.getPosition().y,
					m_backgroundSprite.getDimensions().x,
					m_backgroundSprite.getDimensions().y),
				m_parent->getTextScale(), m_parent->getTextColor(),
				m_parent->getFontFileName(), m_horizontalTextAlignment,
				m_verticalTextAlignment, m_parent->getShader());
		}
		m_lastButton.draw();
		m_nextButton.draw();
	}

	void UICarousel::processInput() {
		if (m_highlighted) {
			if (m_horizontalButtonAlignment == ALIGN_LEFT
				|| m_horizontalButtonAlignment == ALIGN_RIGHT) {
				if (Application::Input.getControllerAxisPosition(
					m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_Y)
						< -0.5f && m_timer > (float)Application::Window
							.getTargetUpdatesPerSecond() / 6.0f) {
					if (m_currentValueIndex >= 1) {
						setCurrentValueIndex(m_currentValueIndex - 1);
					}
					else {
						setCurrentValueIndex((unsigned int)m_values.size() - 1);
					}
					m_parent->UIEvent(m_ID, ValueSetEventID);
					m_timer = 0.0f;
					m_lastButton.setClicked(true);
				}
				else if (Application::Input.getControllerAxisPosition(
					m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_Y)
						> 0.5f && m_timer > (float)Application::Window
							.getTargetUpdatesPerSecond() / 6.0f) {
					setCurrentValueIndex((m_currentValueIndex + 1)
						% m_values.size());
					m_parent->UIEvent(m_ID, ValueSetEventID);
					m_timer = 0.0f;
					m_nextButton.setClicked(true);
				}
			}
			else if (m_horizontalButtonAlignment == ALIGN_CENTER) {
				if (Application::Input.getControllerAxisPosition(
					m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X)
					< -0.5f && m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
					if (m_currentValueIndex >= 1) {
						setCurrentValueIndex(m_currentValueIndex - 1);
					}
					else {
						setCurrentValueIndex((unsigned int)m_values.size() - 1);
					}
					m_parent->UIEvent(m_ID, ValueSetEventID);
					m_timer = 0.0f;
					m_lastButton.setClicked(true);
				}
				else if (Application::Input.getControllerAxisPosition(
					m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_X)
						> 0.5f && m_timer > (float)Application::Window
					.getTargetUpdatesPerSecond() / 6.0f) {
					setCurrentValueIndex((m_currentValueIndex + 1)
						% m_values.size());
					m_parent->UIEvent(m_ID, ValueSetEventID);
					m_timer = 0.0f;
					m_nextButton.setClicked(true);
				}
			}
		}
		if (m_timer > (float)Application::Window
			.getTargetUpdatesPerSecond() / 4.0f) {
			m_timer = 0.0f;
			m_lastButton.setClicked(false);
			m_nextButton.setClicked(false);
		}
		m_lastButton.processInput();
		m_nextButton.processInput();
	}

	void UICarousel::UIEvent(unsigned int componentID, unsigned int eventID) {
		if (componentID == m_lastButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				if (m_currentValueIndex >= 1) {
					setCurrentValueIndex(m_currentValueIndex - 1);
				}
				else {
					setCurrentValueIndex((unsigned int)m_values.size() - 1);
				}
				m_parent->UIEvent(m_ID, ValueSetEventID);
			}
		}
		else if (componentID == m_nextButton.getID()) {
			if (eventID == UIButton::ClickedEventID) {
				if (m_values.empty()) {
					return;
				}
				setCurrentValueIndex((m_currentValueIndex + 1)
					% m_values.size());
				m_parent->UIEvent(m_ID, ValueSetEventID);
			}
		}
	}

	void UICarousel::update(float timeStep) {
		m_lastButton.update(timeStep);
		m_nextButton.update(timeStep);
		m_timer += timeStep;
		if (m_timer > (float)Application::Window.getTargetUpdatesPerSecond()) {
			m_timer = 0.0f;
		}
	}

	void UICarousel::destroy() {
		UILabel::destroy();
		m_values.clear();
		m_currentValueIndex = 0;
		m_lastButton.destroy();
		m_nextButton.destroy();
		m_horizontalTextAlignment = NO_ALIGNMENT;
		m_verticalTextAlignment = NO_ALIGNMENT;
		m_horizontalButtonAlignment = NO_ALIGNMENT;
	}

	// Implement UIListBox class functions

	void UIListBox::initialize(const glm::vec3& position,
		const glm::vec2& dimensions, const std::string& textureFileName,
		const std::string& labelText, const std::vector<std::string>& values,
		unsigned int valueCount, const std::string& valueTextureFileName,
		Alignment labelAlignment, Alignment horizontalLabelAlignment,
		Alignment verticalLabelAlignment, Alignment horizontalTextAlignment,
		Alignment verticalTextAlignment) {
		UILabel::initialize(position, dimensions, textureFileName, labelText,
			labelAlignment, horizontalLabelAlignment, verticalLabelAlignment);
		m_valueTextureFileName = valueTextureFileName;
		m_nextID = 0;
		setValueCount(valueCount);
		setTopValueIndex(0);
		for (const std::string& value : values) {
			addValue(value);
		}
		setPosition(position);
		setDimensions(dimensions);
		setHorizontalTextAlignment(horizontalTextAlignment);
		setVerticalTextAlignment(verticalTextAlignment);
	}

	std::vector<std::string> UIListBox::getValues() const {
		std::vector<std::string> values;
		for (const UISwitch& value : m_values) {
			values.push_back(value.getLabelText());
		}
		return values;
	}

	std::string UIListBox::getValue(unsigned int index) const {
		if (index >= m_values.size()) {
			return "";
		}
		return m_values[index].getLabelText();
	}

	int UIListBox::getValueIndex(const std::string& value) const {
		for (int i = 0; i < m_values.size(); i++) {
			if (m_values[i].getLabelText() == value) {
				return i;
			}
		}
		return -1;
	}

	bool UIListBox::addValue(const std::string& value, int index) {
		if (index < 0) {
			m_values.push_back(UISwitch());
			m_values.back().initialize(glm::vec3(), glm::vec2(),
				m_valueTextureFileName, value, ALIGN_CENTER,
				m_horizontalTextAlignment, m_verticalTextAlignment);
			m_values.back().setID(m_nextID++);
			m_values.back().setParent(*this);
		}
		else {
			m_values.insert(m_values.begin() + index, UISwitch());
			m_values[index].initialize(glm::vec3(), glm::vec2(),
				m_valueTextureFileName, value, ALIGN_CENTER,
				m_horizontalTextAlignment, m_verticalTextAlignment);
			m_values[index].setID(m_nextID++);
			m_values[index].setParent(*this);
		}
		setPosition(getPosition());
		setDimensions(getDimensions());
		return true;
	}

	bool UIListBox::removeValue(const std::string& value) {
		bool f = false;
		std::vector<UISwitch>::iterator it = m_values.begin();
		for (; it != m_values.end(); it++) {
			if (it->getLabelText() == value) {
				f = true;
				break;
			}
		}
		if (!f) {
			return false;
		}
		it->destroy();
		m_values.erase(it);
		setPosition(getPosition());
		setDimensions(getDimensions());
		return true;
	}

	bool UIListBox::removeValue(unsigned int index) {
		if (index >= m_values.size()) {
			return false;
		}
		std::vector<UISwitch>::iterator it = m_values.begin() + index;
		it->destroy();
		m_values.erase(it);
		setPosition(getPosition());
		setDimensions(getDimensions());
		return true;
	}

	std::string UIListBox::getCurrentValue() const {
		for (const UISwitch& value : m_values) {
			if (value.isOn()) {
				return value.getLabelText();
			}
		}
		return "";
	}

	void UIListBox::setCurrentValue(const std::string& value) {
		int index = getValueIndex(value);
		if (index == -1) {
			return;
		}
		setCurrentValueIndex((unsigned int)index);
	}

	int UIListBox::getCurrentValueIndex() const {
		for (int i = 0; i < m_values.size(); i++) {
			if (m_values[i].isOn()) {
				return i;
			}
		}
		return -1;
	}

	void UIListBox::setCurrentValueIndex(unsigned int index) {
		if (index >= m_values.size()) {
			return;
		}
		for (UISwitch& value : m_values) {
			value.setOn(false);
		}
		m_values[index].setOn(true);
	}

	unsigned int UIListBox::getValueCount() const {
		return m_valueCount;
	}

	void UIListBox::setValueCount(unsigned int valueCount) {
		m_valueCount = valueCount;
		setPosition(getPosition());
		setDimensions(getDimensions());
		setTopValueIndex(0);
	}

	unsigned int UIListBox::getTopValueIndex() const {
		return m_topValueIndex;
	}

	void UIListBox::setTopValueIndex(unsigned int topValueIndex) {
		if (m_values.size() < m_valueCount && topValueIndex > 0) {
			return;
		}
		if (topValueIndex > m_values.size() - m_valueCount) {
			return;
		}
		m_topValueIndex = topValueIndex;
		setPosition(getPosition());
		setDimensions(getDimensions());
	}

	Alignment UIListBox::getHorizontalTextAlignment() const {
		return m_horizontalTextAlignment;
	}

	void UIListBox::setHorizontalTextAlignment(
		Alignment horizontalTextAlignment) {
		m_horizontalTextAlignment = horizontalTextAlignment;
		for (UISwitch& value : m_values) {
			value.setHorizontalLabelAlignment(horizontalTextAlignment);
		}
	}

	Alignment UIListBox::getVerticalTextAlignment() const {
		return m_verticalTextAlignment;
	}

	void UIListBox::setVerticalTextAlignment(Alignment verticalTextAlignment) {
		m_verticalTextAlignment = verticalTextAlignment;
		for (UISwitch& value : m_values) {
			value.setVerticalLabelAlignment(verticalTextAlignment);
		}
	}

	void UIListBox::setEnabled(bool enabled) {
		UILabel::setEnabled(enabled);
		for (UISwitch& value : m_values) {
			value.setEnabled(enabled);
		}
	}

	void UIListBox::setPosition(const glm::vec3& position) {
		UILabel::setPosition(position);
		float vx = m_backgroundSprite.getPosition().x
			/ (float)Application::Window.getDimensions().x;
		float vph = m_backgroundSprite.getDimensions().y / (float)m_valueCount;
		float vh = vph / (float)Application::Window.getDimensions().y;
		float vy = ((m_backgroundSprite.getPosition().y
			+ m_backgroundSprite.getDimensions().y - vph)
			+ (vph * m_topValueIndex))
			/ (float)Application::Window.getDimensions().y;
		for (int i = 0; i < m_values.size(); i++) {
			m_values[i].setPosition(glm::vec3(vx, vy,
				m_backgroundSprite.getPosition().z + 0.1f));
			vy -= vh;
		}
	}

	void UIListBox::setDimensions(const glm::vec2& dimensions) {
		UILabel::setDimensions(dimensions);
		float vw = m_backgroundSprite.getDimensions().x
			/ (float)Application::Window.getDimensions().x;
		float vh = (m_backgroundSprite.getDimensions().y / (float)m_valueCount)
			/ (float)Application::Window.getDimensions().y;
		for (UISwitch& value : m_values) {
			value.setDimensions(glm::vec2(vw, vh));
		}
		setPosition(getPosition());
	}

	void UIListBox::UIEvent(unsigned int componentID, unsigned int eventID) {
		for (UISwitch& value : m_values) {
			if (componentID == value.getID()) {
				value.setOn(true);
			}
			else {
				value.setOn(false);
			}
		}
		m_parent->UIEvent(m_ID, ValueSetEventID);
	}

	GLSLShader& UIListBox::getShader() {
		return m_parent->getShader();
	}

	void UIListBox::setShader(const GLSLShader&) {

	}

	const std::string& UIListBox::getFontFileName() const {
		return m_parent->getFontFileName();
	}

	void UIListBox::setFontFileName(const std::string&) {

	}

	float UIListBox::getTextScale() const {
		return m_parent->getTextScale();
	}

	void UIListBox::setTextScale(float) {

	}

	const glm::vec4& UIListBox::getTextColor() const {
		return m_parent->getTextColor();
	}

	void UIListBox::setTextColor(const glm::vec4&) {

	}

	Controller UIListBox::getController() const {
		return m_parent->getController();
	}

	void UIListBox::setController(Controller) {

	}
	
	void UIListBox::draw() {
		UILabel::draw();
		for (unsigned int i = m_topValueIndex;
			i < m_valueCount + m_topValueIndex; i++) {
			if (i < m_values.size()) {
				m_values[i].draw();
			}
		}
	}

	void UIListBox::processInput() {
		for (UISwitch& value : m_values) {
			value.processInput();
		}
		if (IsMouseOver(*this)) {
			float scroll = Application::Input.getMouseScrollMovement().y;
			if (scroll != 0.0f && m_timer
				> Application::Window.getTargetUpdatesPerSecond() / 10.0f) {
				if (scroll < 0.0f) {
					setTopValueIndex(getTopValueIndex() + 1);
				}
				else {
					setTopValueIndex(getTopValueIndex() - 1);
				}
				m_timer = 0.0f;
			}
		}
		if (m_highlighted) {
			if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_Y)
					< -0.5f && m_timer > (float)Application::Window
						.getTargetUpdatesPerSecond() / 6.0f) {
				setCurrentValueIndex(getCurrentValueIndex() - 1);
				setTopValueIndex(getCurrentValueIndex());
				m_timer = 0.0f;
			}
			else if (Application::Input.getControllerAxisPosition(
				m_parent->getController(), CONTROLLER_AXIS_LEFT_STICK_Y)
					> 0.5f && m_timer > (float)Application::Window
						.getTargetUpdatesPerSecond() / 6.0f) {
				setCurrentValueIndex(getCurrentValueIndex() + 1);
				setTopValueIndex(getCurrentValueIndex());
				m_timer = 0.0f;
			}
		}
	}

	void UIListBox::update(float timeStep) {
		m_timer += timeStep;
		if (m_timer > Application::Window.getTargetUpdatesPerSecond()) {
			m_timer = 0.0f;
		}
	}

	void UIListBox::destroy() {
		UILabel::destroy();
		m_valueTextureFileName = "";
		for (UISwitch& value : m_values) {
			value.destroy();
		}
		m_values.clear();
		m_nextID = 0;
		m_valueCount = 0;
		m_topValueIndex = 0;
		m_timer = 0.0f;
		m_horizontalTextAlignment = NO_ALIGNMENT;
		m_verticalTextAlignment = NO_ALIGNMENT;
	}
}