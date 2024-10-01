/*
File:		UIComponents.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.22@19:46
Purpose:	Contains classes representing individual UI components
*/

#ifndef ACTIASFW_UI_COMPONENTS_H
#define ACTIASFW_UI_COMPONENTS_H

#include "UISystem.h"

namespace ActiasFW {
	// A text label UI component
	class UILabel : public UIComponent {
	public:
		/*
		Initialize this label's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this label relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this label relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture for the background of this label
		Parameter: const std::string& labelText - The text to display on this
		label
		Parameter: Alignment labelAlignment - The alignment of this label's text
		about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this label's text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this label's text
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&, Alignment = ALIGN_CENTER,
			Alignment = ALIGN_CENTER, Alignment = ALIGN_CENTER);
		/*
		Get the normalized position and depth of this label relative to the
		application's window dimensions
		Returns: glm::vec3 - This label's position and depth
		*/
		virtual glm::vec3 getPosition() const override;
		/*
		Set the normalized position and depth of this label relative to the
		application's window dimensions
		Parameter: const glm::vec3& position - This label's new position and
		depth
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Get the normalized dimensions of this label relative to the
		application's window dimensions
		Returns: glm::vec2 - This label's dimensions
		*/
		virtual glm::vec2 getDimensions() const override;
		/*
		Set the normalized dimensions of this label relative to the
		application's window dimensions
		Parameter: const glm::vec2& dimensions - This label's new dimensions
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		Get the sprite representing the background of this label
		Returns: Sprite& - This label's background
		*/
		virtual Sprite& getBackgroundSprite();
		/*
		Set the sprite representing the background of this label
		Parameter: const Sprite& backgroundSprite - This label's new background
		*/
		virtual void setBackgroundSprite(const Sprite&);
		/*
		Get the text appearing on this label
		Returns: const std::string& - This label's text
		*/
		virtual const std::string& getLabelText() const;
		/*
		Set the text appearing on this label
		Parameter: const std::string& labelText - This label's new text
		*/
		virtual void setLabelText(const std::string&);
		/*
		Get the alignment of this label's text about its background
		Returns: Alignment - This label's text alignment
		*/
		virtual Alignment getLabelAlignment() const;
		/*
		Set the alignment of this label's text about its background
		Parameter: Alignment labelAlignment - This label's new text alignment
		*/
		virtual void setLabelAlignment(Alignment);
		/*
		Get the horizontal alignment of this label's text
		Returns: Alignment - This label's horizontal text alignment
		*/
		virtual Alignment getHorizontalLabelAlignment() const;
		/*
		Set the horizontal alignment of this label's text
		Parameter: Alignment horizontalLabelAlignment - This label's new
		horizontal text alignment
		*/
		virtual void setHorizontalLabelAlignment(Alignment);
		/*
		Get the vertical alignment of this label's text
		Returns: Alignment - This label's vertical text alignment
		*/
		virtual Alignment getVerticalLabelAlignment() const;
		/*
		Set the vertical alignment of this label's text
		Parameter: Alignment verticalLabelAlignment - This label's new vertical
		text alignment
		*/
		virtual void setVerticalLabelAlignment(Alignment);

	protected:
		// The sprite representing this label's background
		Sprite m_backgroundSprite;
		// The text appearing on this label
		std::string m_labelText = "";
		// The alignment of this label's text about its background
		Alignment m_labelAlignment = NO_ALIGNMENT;
		// The horizontal alignment of this label's text
		Alignment m_horizontalLabelAlignment = NO_ALIGNMENT;
		// The vertical alignment of this label's text
		Alignment m_verticalLabelAlignment = NO_ALIGNMENT;

		/*
		Draw this label's background and text
		*/
		virtual void draw() override;
		/*
		Process user input to this label (not used)
		*/
		virtual void processInput() override;
		/*
		Update this label's logic
		Parameter: float - Unused number of frames elapsed since the last update
		*/
		virtual void update(float) override;
		/*
		Free this label's memory
		*/
		virtual void destroy() override;
	};

	// A clickable button UI component
	class UIButton : public UILabel {
	public:
		// Event ID number for buttons being unselected
		const static unsigned int UnselectedEventID = 0;
		// Event ID number for buttons being selected (moused over)
		const static unsigned int SelectedEventID = 1;
		// Event ID number of buttons being clicked
		const static unsigned int ClickedEventID = 2;

		/*
		Initialize this button's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this button relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this button relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture for the background of this button containing three frames left
		to right for unselected, selected, and clicked button states
		Parameter: const std::string& labelText - The text to display on this
		button
		Parameter: Alignment labelAlignment - The alignment of this button's
		text about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this button's text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this button's text
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&, Alignment = ALIGN_CENTER,
			Alignment = ALIGN_CENTER, Alignment = ALIGN_CENTER) override;
		/*
		Test whether this button is selected (moused over)
		Returns: bool - Whether this button is selected
		*/
		virtual bool isSelected() const;
		/*
		Set whether this button is selected
		Parameter: bool selected - Whether this button should be selected
		*/
		virtual void setSelected(bool);
		/*
		Test whether this button is clicked (mouse over with mouse button down)
		Returns: bool - Whether this button is clicked
		*/
		virtual bool isClicked() const;
		/*
		Set whether this button is clicked
		Parameter: bool clicked - Whether this button should be clicked
		*/
		virtual void setClicked(bool);
		/*
		Set whether this button is enabled
		Parameter: bool enabled - Whether this button should be enabled
		*/
		virtual void setEnabled(bool) override;
		
	protected:
		// Allow the UICarousel class to access protected memory
		friend class UICarousel;
		// Texture box for unselected buttons
		static glm::vec4 UnselectedTextureBox;
		// Texture box for selected (moused over) buttons
		static glm::vec4 SelectedTextureBox;
		// Texture box for clicked buttons
		static glm::vec4 ClickedTextureBox;
		// Whether this button is selected
		bool m_selected = false;
		// Whether this button is clicked
		bool m_clicked = false;

		/*
		Process user input to this button
		*/
		virtual void processInput() override;
		/*
		Free this button's memory
		*/
		virtual void destroy() override;
	};

	// A toggleable switch UI component
	class UISwitch : public UIButton {
	public:
		// Event ID number for switches being toggled off
		const static unsigned int OffEventID = 0;
		// Event ID number for switches being toggled on
		const static unsigned int OnEventID = 1;

		/*
		Initialize this switch's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this switch relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this switch relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture for the background of this switch containing two frames left
		to right for off and on switch states
		Parameter: const std::string& labelText - The text to display on this
		switch
		Parameter: Alignment labelAlignment - The alignment of this switch's
		text about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this switch's text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this switch's text
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&, Alignment = ALIGN_CENTER,
			Alignment = ALIGN_CENTER, Alignment = ALIGN_CENTER) override;
		/*
		Test whether this switch is toggled on
		Returns: bool - Whether this switch is on
		*/
		virtual bool isOn() const;
		/*
		Set whether this switch is toggled on or off
		Parameter: bool on - Whether this switch should be on
		*/
		virtual void setOn(bool);

	protected:
		// Allow the UIListBox class to access protected memory
		friend class UIListBox;
		// Texture box for toggled off switches
		static glm::vec4 OffTextureBox;
		// Texture box for toggled on switches
		static glm::vec4 OnTextureBox;
		// Whether this switch is toggled on or off
		bool m_on = false;

		/*
		Process user input to this switch
		*/
		virtual void processInput() override;
		/*
		Free this switch's memory
		*/
		virtual void destroy() override;
	};

	// A slider UI component
	class UISlider : public UILabel {
	public:
		// Event ID number for sliders being unselected
		const static unsigned int UnselectedEventID = 0;
		// Event ID number for sliders being selected
		const static unsigned int SelectedEventID = 1;
		// Event ID number for sliders' values being set
		const static unsigned int ValueSetEventID = 2;

		/*
		Initialize this slider's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this slider relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this slider relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture for the background of this slider containing two frames left to
		right for unselected and selected slider states
		Parameter: const glm::vec2& cursorDimensions - The dimensions of this
		slider's cursor in pixels
		Parameter: const std::string& cursorTextureFileName - The file name of
		the texture of this slider's cursor
		Parameter: const std::string& labelText - The text to appear in this
		slider
		Parameter: float value - The initial value of this slider in percent
		Parameter: Alignment labelAlignment - The alignment of this slider's
		label about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this slider's label text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this slider's label text
		Parameter: Alignment verticalCursorAlignment - The vertical alignment of
		this slider's cursor on its background
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const glm::vec2&, const std::string&,
			const std::string&, float, Alignment = ALIGN_TOP,
			Alignment = ALIGN_LEFT, Alignment = ALIGN_BOTTOM,
			Alignment = ALIGN_CENTER);
		/*
		Test whether this slider is currently selected
		Returns: bool - Whether this slider is selected
		*/
		virtual bool isSelected() const;
		/*
		Set whether this slider is currently selected
		Parameter: bool selected - Whether this slider should be selected
		*/
		virtual void setSelected(bool);
		/*
		Get the current value of this slider in percent
		Returns: float - This slider's value
		*/
		virtual float getValue() const;
		/*
		Set the value of this slider
		Parameter: float value - The new value for this slider in percent
		*/
		virtual void setValue(float);
		/*
		Get the sprite representing this slider's cursor
		Returns: Sprite& - This slider's cursor
		*/
		virtual Sprite& getCursor();
		/*
		Set the sprite representing this slider's cursor
		Parameter: const Sprite& cursor - This slider's new cursor
		*/
		virtual void setCursor(const Sprite&);
		/*
		Get the vertical alignment of this slider's cursor on its background
		Returns: Alignment - This slider's cursor's vertical alignment
		*/
		virtual Alignment getVerticalCursorAlignment() const;
		/*
		Set the vertical alignment of this slider's cursor on its background
		Parameter: Alignment verticalCursorAlignment - This slider's cursor's
		new vertical alignment
		*/
		virtual void setVerticalCursorAlignment(Alignment);
		/*
		Set whether this slider is enabled
		Parameter: bool enabled - Whether this slider should be enabled
		*/
		virtual void setEnabled(bool) override;
		/*
		Set the normalized dimensions of this slider relative to the
		application's window dimensions
		Parameter: const glm::vec2& dimensions - This slider's new dimensions
		*/
		virtual void setDimensions(const glm::vec2&) override;
	
	protected:
		// The texture box for unselected sliders
		static glm::vec4 UnselectedTextureBox;
		// The texture box for selected sliders
		static glm::vec4 SelectedTextureBox;
		// Whether this slider is currently selected
		bool m_selected = false;
		// The current value of this slider in percent
		float m_value = 0.0f;
		// The sprite representing this slider's cursor
		Sprite m_cursor;
		// The vertical alignment of this slider's cursor on its background
		Alignment m_verticalCursorAlignment = NO_ALIGNMENT;

		/*
		Draw this slider's graphics
		*/
		virtual void draw() override;
		/*
		Process user input to this slider
		*/
		virtual void processInput() override;
		/*
		Update this slider's logic
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Free this slider's memory
		*/
		virtual void destroy();
	};

	// A text box UI component
	class UITextBox : public UILabel {
	public:
		// Event ID number for text boxes being unselected
		const static unsigned int UnselectedEventID = 0;
		// Event ID number for text boxes being selected
		const static unsigned int SelectedEventID = 1;
		// Event ID number for text being entered into text boxes
		const static unsigned int TextEnteredEventID = 2;

		/*
		Initialize this text box's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this text box relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this text box relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture to appear as this text box's background containing two frames
		left to right for unselected and selected text box states
		Parameter: const glm::vec2& cursorDimensions - The dimensions of this
		text box's cursor in pixels
		Parameter: const std::string& cursorTextureFileName - The file name of
		the texture for this text box's cursor
		Parameter: const std::string& labelText - The text to appear in this
		text box's label
		Parameter: const std::string& text - The initial text to appear in this
		text box
		Parameter: const std::string& allowedCharacters - The set of characters
		allowed in this text box
		Parameter: unsigned int maximumCharacters - The maximum number of
		characters allowed in this text box
		Parameter: Alignment labelAlignment - The alignment of this text box's
		label about its background
		Parameter: Alignment horizontalAlignment - The horizontal alignment of
		this text box's label text
		Parameter: Alignment verticalAlignment - The vertical alignment of this
		text box's label text
		Parameter: Alignment verticalTextAlignment - The vertical alignment of
		this text box's text on its background
		Parameter: Alignment verticalCursorAlignment - The vertical alignment of
		this text box's cursor on its background
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const glm::vec2&, const std::string&,
			const std::string&, const std::string&, const std::string& = "",
			int = -1, Alignment = ALIGN_TOP, Alignment = ALIGN_LEFT,
			Alignment = ALIGN_BOTTOM, Alignment = ALIGN_CENTER,
			Alignment = ALIGN_CENTER);
		/*
		Test whether this text box is currently selected
		Returns: bool - Whether this text box is selected
		*/
		virtual bool isSelected() const;
		/*
		Set whether this text box is selected
		Parameter: bool selected - Whether this text box should be selected
		*/
		virtual void setSelected(bool);
		/*
		Get the text currently appearing in this text box
		Returns: const std::string& - This text box's content text
		*/
		virtual const std::string& getText() const;
		/*
		Set the content text of this text box
		Parameter: const std::string& text - This text box's new content text
		*/
		virtual void setText(const std::string&);
		/*
		Get the set of characters allowed in this text box's content
		Returns: const std::string& - This text box's allowed characters
		*/
		virtual const std::string& getAllowedCharacters() const;
		/*
		Set the set of characters allowed in this text box's content
		Parameter: const std::string& allowedCharacters - This text box's new
		allowed characters
		*/
		virtual void setAllowedCharacters(const std::string&);
		/*
		Get the maximum number of characters allowed in this text box's content
		Returns: int - This text box's maximum characters
		*/
		virtual int getMaximumCharacters() const;
		/*
		Set the maximum number of characters allowed in this text box's content
		Parameter: int maximumCharacters - This text box's new maximum
		characters
		*/
		virtual void setMaximumCharacters(int);
		/*
		Get the vertical alignment of this text box's content text on its
		background
		Returns: Alignment - This text box's vertical text alignment
		*/
		virtual Alignment getVerticalTextAlignment() const;
		/*
		Set the vertical alignment of this text box's content text on its
		background
		Parameter: Alignment verticalTextAlignment - This text box's new
		vertical text alignment
		*/
		virtual void setVerticalTextAlignment(Alignment);
		/*
		Get the sprite represeting this text box's cursor
		Returns: Sprite& - This text box's cursor
		*/
		virtual Sprite& getCursor();
		/*
		Set the sprite representing this text box's cursor
		Parameter: const Sprite& cursor - This text box's new cursor
		*/
		virtual void setCursor(const Sprite&);
		/*
		Get the position of this text box's cursor in its content text
		Returns: unsigned int - This text box's cursor position
		*/
		virtual unsigned int getCursorPosition() const;
		/*
		Set the position of this text box's cursor in its content text
		Parameter: unsigned int cursorPosition - This text box's new cursor
		position
		*/
		virtual void setCursorPosition(unsigned int);
		/*
		Get the vertical alignment of this text box's cursor on its background
		Returns: Alignment - This text box's vertical cursor alignment
		*/
		virtual Alignment getVerticalCursorAlignment() const;
		/*
		Set the vertical alignment of this text box's cursor on its background
		Parameter: Alignment - This text box's new vertical cursor alignment
		*/
		virtual void setVerticalCursorAlignment(Alignment);
		/*
		Set this text box's normalized positon and depth relative to the
		application's window dimensions
		Parameter: const glm::vec3& position - The new position and depth of
		this text box
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Set this text box's normalized dimensions relative to the application's
		window dimensions
		Parameter: const glm::vec2& dimensions - The new dimensions of this text
		box
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		Set whether this text box is enabled
		Parameter: bool - Whether this text box should be enabled
		*/
		virtual void setEnabled(bool) override;

	protected:
		// Texture box for unselected text boxes
		static glm::vec4 UnselectedTextureBox;
		// Texture box for selected text boxes
		static glm::vec4 SelectedTextureBox;
		// Whether this text box is currently selected
		bool m_selected = false;
		// This text box's content text
		std::string m_text = "";
		// The set of allowed characters in this text box's content
		std::string m_allowedCharacters = "";
		// The maximum number of characters allowed in this text box's content
		int m_maximumCharacters = 0;
		// The horizontal position in pixels of this text box's content text
		float m_horizontalTextPosition = 0.0f;
		// The vertical alignment of this text box's text on its background
		Alignment m_verticalTextAlignment = NO_ALIGNMENT;
		// The sprite representing this text box's cursor
		Sprite m_cursor;
		// The position of this text box's cursor in its content text
		unsigned int m_cursorPosition = 0;
		// The vertical alignment of this text box's cursor on its background
		Alignment m_verticalCursorAlignment = NO_ALIGNMENT;
		// Timer used for repeated user inputs to this text box
		float m_timer = 0.0f;
		// Whether this text box was highlighted in the last update
		bool m_lastHighlighted = false;

		/*
		Draw this text box's graphics
		*/
		virtual void draw() override;
		/*
		Process user input to this text box
		*/
		virtual void processInput() override;
		/*
		Update this text box's logic
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Free this text box's memory
		*/
		virtual void destroy() override;
		/*
		Input callback for characters typed into this text box
		Parameter: char character - The character typed
		*/
		virtual void characterTyped(char) override;
	};

	// A password input box UI component
	class UIPasswordBox : public UITextBox {
	protected:
		/*
		Draw this password box's graphics
		*/
		virtual void draw() override;
		/*
		Process user input to this password box
		*/
		virtual void processInput() override;
		/*
		Update this password box's logic
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Free this password box's memory
		*/
		virtual void destroy() override;
	};

	// A multi-line text area UI component
	class UITextArea : public UITextBox {
	public:
		/*
		Initialize this text area's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this text area relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this text area relative to the application's window dimensions
		Parameter: unsigned int lineCount - The number of lines of text to be
		shown on this text area's background
		Parameter: const std::string& textureFileName - The file name of the
		texture of this text area's background containing two frames left to
		right for unselected and selected text area states
		Parameter: const glm::vec2& cursorDimensions - The dimensions of this
		text area's cursor in pixels
		Parameter: const std::string& cursorTextureFileName - The texture of
		this text area's cursor
		Parameter: const std::string& labelText - The text to appear in this
		text area's label
		Parameter: std::string& text - The initial content text of this text
		area
		Parameter: const std::string& allowedCharacters - The set of characters
		allowed in this text area's content text
		Parameter: int maximumCharacters - The maximum number of characters
		allowed in this text area's content text
		Parameter: Alignment labelAlignment - The alignment of this text area's
		label about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this text area's label text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this text area's label text
		Parameter: Alignment verticalCursorAlignment - The vertical alignment of
		this text area's cursor on its content text lines
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			unsigned int, const std::string&, const glm::vec2&,
			const std::string&, const std::string&, const std::string&,
			const std::string& = "", int = -1, Alignment = ALIGN_TOP,
			Alignment = ALIGN_LEFT, Alignment = ALIGN_BOTTOM,
			Alignment = ALIGN_CENTER);
		/*
		Set the normalized position and depth of this text area relative to the
		application's window dimensions
		Parameter: const glm::vec3& position - This text area's new position and
		depth
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Set the normalized dimensions of this text area relative to the
		application's window dimensions
		Parameter: const glm::vec2& dimensions - This text area's new dimensions
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		Set this text area's content text
		Parameter: const std::string& text - This text area's new content text
		*/
		virtual void setText(const std::string&) override;
		/*
		Get the number of lines of text shown on this text area's background
		Returns: unsigned int - This text area's line count
		*/
		virtual unsigned int getLineCount() const;
		/*
		Set the number of lines of text shown on this text area's background
		Parameter: unsigned int lineCount - This text area's new line count
		*/
		virtual void setLineCount(unsigned int);
		/*
		Get the index of the first line of text appearing in this text area
		Returns: int - This text area's top line
		*/
		virtual int getTopLineIndex() const;
		/*
		Set the index of the first line of text appearing in this text area
		Parameter: int topLineIndex - This text area's new top line
		*/
		virtual void setTopLineIndex(int);

	protected:
		// The number of lines appearing in this text area
		unsigned int m_lineCount = 0;
		// The index of the top line of text appearing in this text area
		int m_topLineIndex = 0;

		/*
		Draw this text area's graphics
		*/
		virtual void draw() override;
		/*
		Process user input to this text area
		*/
		virtual void processInput() override;
		/*
		Update this text area's logic
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Free this text area's memory
		*/
		virtual void destroy() override;
		/*
		Callback function for text typed into this text area
		Parameter: char character - The character typed
		*/
		virtual void characterTyped(char) override;

	private:
		/*
		Move the cursor down one line
		*/
		void moveCursorDown();
		/*
		Move the cursor up one line
		*/
		void moveCursorUp();
		/*
		Update the top line and text positioning of this text area to position
		the cursor on the background
		*/
		void putCursorInView();
		/*
		Get the lines of text appearing in this text area
		Returns: std::vector<std::string> - The content text lines
		*/
		std::vector<std::string> getContentLines() const;
	};

	// A carousel UI component
	class UICarousel : public UILabel, UIListener {
	public:
		// Event ID number for carousel values being changed
		const static unsigned int ValueSetEventID = 0;

		/*
		Initialize this carousel's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this carousel relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this carousel relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture to show on this carousel's background
		Parameter: const std::string& labelText - The text to appear in the
		label of this carousel
		Parameter: const std::vector<std::string>& values - The initial set of
		values to appear in this carousel
		Parameter: const std::string& value - The initial value to appear in
		this carousel
		Parameter: float buttonWidth - The normalized width of the last and next
		buttons of this carousel relative to its background
		Parameter: const std::string& buttonTextureFileName - The file name of
		the texture to appear on this carousel's last and next buttons
		containing three frames left to right for unselected, selected, and
		clicked button states
		Parameter: Alignment labelAlignment - The alignment of this carousel's
		label about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this carousel's label text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this carousel's label text
		Parameter: Alignment horizontalTextAlignment - The horizontal alignment
		of this carousel's value on its background
		Parameter: Alignment verticalTextAlignment - The vertical alignment of
		this carousel's value on its background
		Parameter: Alignment horizontalButtonAlignment - The horizontal
		alignment of this carousel's last and next buttons about its background
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			const std::vector<std::string>&, const std::string&,
			float, const std::string&, Alignment = ALIGN_TOP,
			Alignment = ALIGN_LEFT, Alignment = ALIGN_BOTTOM,
			Alignment = ALIGN_CENTER, Alignment = ALIGN_CENTER,
			Alignment = ALIGN_CENTER);
		/*
		Get the set of values in this carousel
		Returns: const std::vector<std::string>& - This carousel's values
		*/
		virtual const std::vector<std::string>& getValues() const;
		/*
		Get the value of this carousel at a given index
		Parameter: unsigned int index - The index to get
		Returns: const std::string - This carousel's value at the given index or
		empty string
		*/
		virtual const std::string getValue(unsigned int) const;
		/*
		Get the index of a value in this carousel
		Parameter: const std::string& value - The value to get the index of
		Returns: int - The index of the given value or -1
		*/
		virtual int getValueIndex(const std::string&) const;
		/*
		Add a value to this carousel at a given index
		Parameter: const std::string& value - The value to add
		Parameter: int index - The index at which to add the value or -1 for any
		Returns: bool - Whether the value was not already present and could be
		added
		*/
		virtual bool addValue(const std::string&, int = -1);
		/*
		Remove a value from this carousel
		Parameter: const std::string& value - The value to remove
		Returns: bool - Whether the value was present and could be removed
		*/
		virtual bool removeValue(const std::string&);
		/*
		Remove a value from this carousel by its index
		Parameter: unsigned int index - The index of the value to remove
		Returns: bool - Whether the value was present and could be removed
		*/
		virtual bool removeValue(unsigned int);
		/*
		Remove all values from this carousel
		*/
		virtual void clear();
		/*
		Get the current value of this carousel
		Returns: const std::string& - This carousel's current value
		*/
		virtual const std::string& getCurrentValue() const;
		/*
		Set the current value of this carousel
		Parameter: const std::string& value - This carousel's new current value
		*/
		virtual void setCurrentValue(const std::string&);
		/*
		Get the index of the current value of this carousel
		Returns: unsigned int - The index of this carousel's current value
		*/
		virtual unsigned int getCurrentValueIndex() const;
		/*
		Set the index of the current value of this carousel
		Parameter: unsigned int index - The new index of this carousel's current
		value
		*/
		virtual void setCurrentValueIndex(unsigned int);
		/*
		Get this carousel's last value button
		Returns: UIButton& - This carousel's last button
		*/
		UIButton& getLastButton();
		/*
		Set this carousel's last value button
		Parameter: const UIButton& lastButton - This carousel's new last button
		*/
		void setLastButton(const UIButton&);
		/*
		Get this carousel's next value button
		Returns: UIButton& - This carousel's next button
		*/
		UIButton& getNextButton();
		/*
		Set this carousel's next value button
		Parameter: const UIButton& lastButton - This carousel's new next button
		*/
		void setNextButton(const UIButton&);
		/*
		Get the horizontal alignment of this carousel's value text on its
		background
		Returns: Alignment - This carousel's horizontal text alignment
		*/
		virtual Alignment getHorizontalTextAlignment() const;
		/*
		Set the horizontal alignment of this carousel's value text on its
		background
		Parameter: Alignment horizontalTextAlignment - This carousel's new
		horizontal text alignment
		*/
		virtual void setHorizontalTextAlignment(Alignment);
		/*
		Get the vertical alignment of this carousel's value text on its
		background
		Returns: Alignment - This carousel's vertical text alignment
		*/
		virtual Alignment getVerticalTextAlignment() const;
		/*
		Set the vertical alignment of this carousel's value text on its
		background
		Parameter: Alignment verticalTextAlignment - This carousel's new
		vertical text alignment
		*/
		virtual void setVerticalTextAlignment(Alignment);
		/*
		Get the horizontal alignment of this carousel's last and next buttons
		about its background
		Returns: Alignment - This carousel's horizontal button alignment
		*/
		virtual Alignment getHorizontalButtonAlignment() const;
		/*
		Set the horizontal alignment of this carousel's last and next buttons
		about its background
		Parameter: Alignment horizontalButtonAlignment - This carousel's new
		horizontal button alignment
		*/
		virtual void setHorizontalButtonAlignment(Alignment);
		/*
		Get the normalized position and depth of this carousel relative to the
		application's window dimensions
		Returns: glm::vec3 - This carousel's position and depth
		*/
		virtual glm::vec3 getPosition() const override;
		/*
		Set the normalized position and depth of this carousel relative to the
		application's window dimensions
		Parameter: const glm::vec3& position - This carousel's new position and
		depth
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Get the normalized dimensions of this carousel relative to the
		application's window dimensions
		Returns: glm::vec2 - This carousel's dimensions
		*/
		virtual glm::vec2 getDimensions() const override;
		/*
		Set the normalized dimensions of this carousel relative to the
		application's window dimensions
		Parameter: const glm::vec2& dimensions - This carousel's new dimensions
		*/
		virtual void setDimensions(const glm::vec2&) override;
		/*
		Set whether this carousel and its buttons are enabled
		Parameter: bool - Whether this carousel should be enabled
		*/
		virtual void setEnabled(bool) override;

	protected:
		// The values in this carousel
		std::vector<std::string> m_values;
		// The index of this carousel's current value
		unsigned int m_currentValueIndex = 0;
		// This carousel's last value button
		UIButton m_lastButton;
		// This carousel's next value button
		UIButton m_nextButton;
		// The horizontal alignment of this carousel's value text
		Alignment m_horizontalTextAlignment = NO_ALIGNMENT;
		// The vertical alignment of this carousel's value text
		Alignment m_verticalTextAlignment = NO_ALIGNMENT;
		// The horizontal alignment of this carousel's buttons about its
		// background
		Alignment m_horizontalButtonAlignment = NO_ALIGNMENT;
		// Timer for controller input
		float m_timer = 0.0f;

		/*
		Get the shader used to draw this carousel's buttons
		Returns: GLSLShader& - This carousel's shader
		*/
		virtual GLSLShader& getShader() override;
		/*
		Set the shader used to draw this carousel's buttons (unused)
		Parameter: const GLSLShader& - Unused shader
		*/
		virtual void setShader(const GLSLShader&) override;
		/*
		Get the file name of the font used to draw this carousel's text
		Returns: const std::string& - This carousel's font file name
		*/
		virtual const std::string& getFontFileName() const override;
		/*
		Set the file name of the font used to draw this carousel's text (unused)
		Parameter: const std::string& - Unused font file name
		*/
		virtual void setFontFileName(const std::string&) override;
		/*
		Get the scale at which text is drawn on this carousel
		Returns: float - This carousel's text scale
		*/
		virtual float getTextScale() const override;
		/*
		Set the scale at which text is drawn on this carousel (unused)
		Parameter: float - Unused text scale
		*/
		virtual void setTextScale(float) override;
		/*
		Get the color in which text is drawn on this carousel
		Returns: const glm::vec4& - This carousel's text color
		*/
		virtual const glm::vec4& getTextColor() const override;
		/*
		Set the color in which text is drawn on this carousel (unused)
		Parameter: const glm::vec4& - This carousel's text color
		*/
		virtual void setTextColor(const glm::vec4&) override;
		/*
		Get the game controller allowed to interact with the components
		controller by this carousel
		Returns: Controller - This carousel's game controller
		*/
		virtual Controller getController() const override;
		/*
		Set the game controller allowed to interact with the components
		controller by this carousel (unused)
		Parameter: Controller - Unused controller
		*/
		virtual void setController(Controller) override;
		/*
		Draw this carousel's graphics and its buttons
		*/
		virtual void draw() override;
		/*
		Process user input to this carousel and its buttons
		*/
		virtual void processInput() override;
		/*
		One of this carousel's buttons has triggered an event
		Parameter: unsigned int componentID - The ID number of the button
		Parameter: unsigned int eventID - The event type ID number
		*/
		virtual void UIEvent(unsigned int, unsigned int) override;
		/*
		Update this carousel's logic and that of its buttons
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Free this carousel's memory and that of its buttons
		*/
		virtual void destroy() override;
	};

	// A list box UI component
	class UIListBox : public UILabel, UIListener {
	public:
		// Event ID number for list box values changed
		const static unsigned int ValueSetEventID = 0;

		/*
		Initialize this list box's memory
		Parameter: const glm::vec3& position - The normalized position and depth
		of this list box relative to the application's window dimensions
		Parameter: const glm::vec2& dimensions - The normalized dimensions of
		this list box relative to the application's window dimensions
		Parameter: const std::string& textureFileName - The file name of the
		texture for this list box's background
		Parameter: const std::string& labelText - The text to appear in this
		list box's label
		Parameter: const std::vector<std::string>& values - The initial set of
		values to appear in this list box
		Parameter: unsigned int valueCount - The number of values which appear
		on the background of this list box
		Parameter: const std::string& valueTextureFileName - The file name of
		the texture to appear on the values in this list box containing two
		frames left to right for off and on switch states
		Parameter: Alignment labelAlignment - The alignment of this list box's
		label about its background
		Parameter: Alignment horizontalLabelAlignment - The horizontal alignment
		of this list box's label text
		Parameter: Alignment verticalLabelAlignment - The vertical alignment of
		this list box's label text
		Parameter: Alignment horizontalTextAlignment - The horizontal alignment
		of the text appearing on this list box's values
		Parameter: Alignment verticalTextAlignment - The vertical alignment of
		the text appearing on this list box's values
		*/
		virtual void initialize(const glm::vec3&, const glm::vec2&,
			const std::string&, const std::string&,
			const std::vector<std::string>&, unsigned int,
			const std::string&, Alignment = ALIGN_TOP, Alignment = ALIGN_LEFT,
			Alignment = ALIGN_BOTTOM, Alignment = ALIGN_CENTER,
			Alignment = ALIGN_CENTER);
		/*
		Get the set of values appearing in this list box
		Returns: std::vector<std::string> - This list box's values
		*/
		virtual std::vector<std::string> getValues() const;
		/*
		Get the value in this list box at a given index or empty string
		Parameter: unsigned int index - The index of the value to get
		Returns: std::string - The value at the given index
		*/
		virtual std::string getValue(unsigned int) const;
		/*
		Get the index of a value in this list box or -1
		Parameter: const std::string& value - The value to get the index of
		Returns: int - The index of the value
		*/
		virtual int getValueIndex(const std::string&) const;
		/*
		Add a value to this list box
		Parameter: const std::string& value - The value to add
		Parameter: int index - The index at which to add the value or -1 for any
		Returns: bool - Whether the value could be added
		*/
		virtual bool addValue(const std::string&, int = -1);
		/*
		Remove the first instance of a value from this list box
		Parameter: const std::string& value - The value to remove
		Returns: bool - Whether the value was present and could be removed
		*/
		virtual bool removeValue(const std::string&);
		/*
		Remove the value at a given index in this list box
		Parameter: unsigned int index - The index of the value to remove
		Returns: bool - Whether the value was present and could be removed
		*/
		virtual bool removeValue(unsigned int);
		/*
		Get the currently selected value in this list box or empty string
		Returns: std::string - This list box's current value
		*/
		virtual std::string getCurrentValue() const;
		/*
		Set the current value selected in this list box
		Parameter: const std::string& value - The value to select
		*/
		virtual void setCurrentValue(const std::string&);
		/*
		Get the index of the currently seleted value in this list box or -1
		Returns: int - The index of this list box's currently selected value
		*/
		virtual int getCurrentValueIndex() const;
		/*
		Set the index of the currently selected value in this list box
		Parameter: unsigned int index - The index of this list box to select
		*/
		virtual void setCurrentValueIndex(unsigned int);
		/*
		Get the number of values appearing on the background of this list box
		Returns: unsigned int - This list box's value count
		*/
		virtual unsigned int getValueCount() const;
		/*
		Set the number of values appearing on the background of this list box
		Parameter: unsigned int valueCount - This list box's new value count
		*/
		virtual void setValueCount(unsigned int);
		/*
		Get the index of the value appearing at the top of this list box
		Returns: unsigned int - This list box's top index
		*/
		virtual unsigned int getTopValueIndex() const;
		/*
		Set the index of the value appearing at the top of this list box
		Parameter: unsigned int topValueIndex - This list box's new top index
		*/
		virtual void setTopValueIndex(unsigned int);
		/*
		Get the horizontal alignment of this list box's values' text
		Returns: Alignment - This list box's horizontal text alignment
		*/
		virtual Alignment getHorizontalTextAlignment() const;
		/*
		Set the horizontal alignment of this list box's values' text
		Parameter: Alignment horizontalTextAlignment - This list box's new
		horizontal text alignment
		*/
		virtual void setHorizontalTextAlignment(Alignment);
		/*
		Get the vertical alignment of this list box's values' text
		Returns: Alignment - This list box's vertical text alignment
		*/
		virtual Alignment getVerticalTextAlignment() const;
		/*
		Set the vertical alignment of this list box's values' text
		Parameter: Alignment verticalTextAlignment - This list box's new
		vertical text alignment
		*/
		virtual void setVerticalTextAlignment(Alignment);
		/*
		Set whether this list box and its values are enabled
		Parameter: bool enabled - Whether to enabled this list box
		*/
		virtual void setEnabled(bool) override;
		/*
		Set the normalized position and depth of this list box relative to the
		application's window dimensions
		Parameter: const glm::vec3& position - This list box's new position
		*/
		virtual void setPosition(const glm::vec3&) override;
		/*
		Set the normalized dimensions of this list box relative to the
		application's window dimensions
		Parameter: const glm::vec2& dimensions - This list box's new dimensions
		*/
		virtual void setDimensions(const glm::vec2&) override;

	protected:
		// The file name of the texture to apply to this list box's values
		std::string m_valueTextureFileName = "";
		// The set of values contained in this list box
		std::vector<UISwitch> m_values;
		// The next ID to assign to a newly added value of this list box
		int m_nextID = 0;
		// The number of values appearing on the background of this list box
		unsigned int m_valueCount = 0;
		// The index of the value appearing at the top of this list box
		unsigned int m_topValueIndex = 0;
		// The timer to use for repeated user inputs
		float m_timer = 0.0f;
		// The horizontal alignment of this list box's values' text
		Alignment m_horizontalTextAlignment = NO_ALIGNMENT;
		// The vertical alignment of this list box's values' text
		Alignment m_verticalTextAlignment = NO_ALIGNMENT;

		/*
		A value in this list box has triggered an event
		Parameter: unsigned int componentID - The ID number of the component
		Parameter: unsigned int eventID - The ID number of the event type
		*/
		virtual void UIEvent(unsigned int, unsigned int) override;
		/*
		Get the shader used to draw this list box and its values
		Returns: GLSLShader& - This list box's shader
		*/
		virtual GLSLShader& getShader() override;
		/*
		Set the shader used to draw this list box and its values (unused)
		Parameter: const GLSLShader& - Unused shader
		*/
		virtual void setShader(const GLSLShader&) override;
		/*
		Get the file name of the font used to draw this list box and its values
		Returns: const std::string& - This list box's font file name
		*/
		virtual const std::string& getFontFileName() const override;
		/*
		Set the file name of the font used to draw this list box and its values
		(unused)
		Parameter: const std::string& - Unused text file name
		*/
		virtual void setFontFileName(const std::string&) override;
		/*
		Get the scale at which to draw text on this list box and its values
		Returns: float - This list box's text scale
		*/
		virtual float getTextScale() const override;
		/*
		Set the scale at which to draw text on this list box and its values
		(unused)
		Parameter: float textScale - Unused text scale
		*/
		virtual void setTextScale(float) override;
		/*
		Get the color in which to draw text on this list box and its values
		Returns: const glm::vec4& - This list box's text color
		*/
		virtual const glm::vec4& getTextColor() const override;
		/*
		Set the color in which to draw text on this list box and its values
		(unused)
		Parameter: const glm::vec4& - Unused text color
		*/
		virtual void setTextColor(const glm::vec4&) override;
		/*
		Get the game controller allowed to interact with the components
		controller by this list box
		Returns: Controller - This list box's game controller
		*/
		virtual Controller getController() const override;
		/*
		Set the game controller allowed to interact with the components
		controller by this list box (unused)
		Parameter: Controller - Unused controller
		*/
		virtual void setController(Controller) override;
		/*
		Draw this list box and its values
		*/
		virtual void draw() override;
		/*
		Process user input to this list box and its values
		*/
		virtual void processInput() override;
		/*
		Update this list box's and its switches' logic
		Parameter: float timeStep - The number of frames elapsed since the last
		update
		*/
		virtual void update(float) override;
		/*
		Free this list box's memory and that of its values
		*/
		virtual void destroy() override;
	};
}

#endif