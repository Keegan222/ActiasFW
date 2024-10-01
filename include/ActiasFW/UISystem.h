/*
File:		UISystem.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.22@19:45
Purpose:	Contains organization/mamagement classes for UI components
*/

#ifndef ACTIASFW_UI_SYSTEM_H
#define ACTIASFW_UI_SYSTEM_H

#include "Graphics.h"
#include "Input.h"

namespace ActiasFW {
	// Forward declare the ActiasFW application's Scene class
	class Scene;

	// An abstract event listener for user interface components
	class UIListener {
	public:
		/*
		A UI component has triggered an event
		Parameter: unsigned int - The ID of the component
		Parameter: unsigned int - The ID of the event type
		*/
		virtual void UIEvent(unsigned int, unsigned int) = 0;
		/*
		Get the shader used to draw the UI components this listener controls
		Returns: GLSLShader& - This UI listener's shader
		*/
		virtual GLSLShader& getShader() = 0;
		/*
		Set the shader used to draw the UI components this listener controls
		Parameter: const GLSLShader& - This UI listener's new shader
		*/
		virtual void setShader(const GLSLShader&) = 0;
		/*
		Get the file name of the font used to draw the UI components this
		listener controls
		Returns: const std::string& - This UI listener's font file name
		*/
		virtual const std::string& getFontFileName() const = 0;
		/*
		Set the file name of the font used to draw the UI components this
		listener controls
		Parameter: const std::string& - This UI listener's new font file name
		*/
		virtual void setFontFileName(const std::string&) = 0;
		/*
		Get the scale with which text is drawn on the UI components this
		listener controls
		Returns: float - This UI listener's text scale
		*/
		virtual float getTextScale() const = 0;
		/*
		Set the scale with which text will be drawn on the UI components this
		listener controls
		Parameter: float - This UI listener's new text scale
		*/
		virtual void setTextScale(float) = 0;
		/*
		Get the color in which text is drawn on the UI components this listener
		controls
		Returns: const glm::vec4& - This UI listener's text color
		*/
		virtual const glm::vec4& getTextColor() const = 0;
		/*
		Set the color with which text will be drawn on the UI components this
		listener controls
		Parameter: const glm::vec4& - This UI listener's text color
		*/
		virtual void setTextColor(const glm::vec4&) = 0;
		/*
		Get the game controller allowed to interact with components this
		listener controls
		Returns: Controller - This listener's game controller
		*/
		virtual Controller getController() const = 0;
		/*
		Set the game controller allowed to interact with components this
		listener controls
		Parameter: Controller controller - This listener's new game controller
		*/
		virtual void setController(Controller) = 0;
	};

	// An abstract user interface component controlled by a UI listener
	class UIComponent : public InputListener {
	public:
		/*
		Get the UI listener which controls this component
		Returns: UIListener& - This component's parent
		*/
		virtual UIListener& getParent();
		/*
		Set the UI listener which controls this component
		Parameter: UIListener& parent - This component's new parent
		*/
		virtual void setParent(UIListener&);
		/*
		Get the ID number of this UI component
		Returns: unsigned int - This component's ID
		*/
		virtual unsigned int getID() const;
		/*
		Set the ID number of this UI component
		Parameter: unsigned int ID - This component's new ID
		*/
		virtual void setID(unsigned int);
		/*
		Test whether this UI component is enabled
		Returns: bool - Whether this component is enabled
		*/
		virtual bool isEnabled() const;
		/*
		Set whether this UI component is enabled
		Parameter: bool enabled - Whether this component should be enabled
		*/
		virtual void setEnabled(bool);
		/*
		Test whether this UI component is visible
		Returns: bool - Whether this component is visible
		*/
		virtual bool isVisible() const;
		/*
		Set whether this UI component is visible
		Parameter: bool visible - Whether this component should be visible
		*/
		virtual void setVisible(bool);
		/*
		Get the normalized position and depth of this UI component relative to
		the application's window dimensions
		Returns: glm::vec3 - This component's position
		*/
		virtual glm::vec3 getPosition() const = 0;
		/*
		Set the normalized position and depth of this UI component relative to
		the application's window dimensions
		Parameter: const glm::vec3& - This component's new position
		*/
		virtual void setPosition(const glm::vec3&) = 0;
		/*
		Get the normalized dimensions of this UI component relative to the
		application's window dimensions
		Returns: glm::vec2 - This component's dimensions
		*/
		virtual glm::vec2 getDimensions() const = 0;
		/*
		Set the normalized dimensions of this UI component relative to the
		application's window dimensions
		Parameter: const glm::vec2& - This component's new dimensions
		*/
		virtual void setDimensions(const glm::vec2&) = 0;
		/*
		Get a reference to the component to the left of this one
		Returns: UIComponent& - The component to the left of this one
		*/
		virtual UIComponent& getNeighbourLeft();
		/*
		Set the component to the left of this one
		Parameter: UIComponent& neighbourLeft - The new component to the left of
		this one
		*/
		virtual void setNeighbourLeft(UIComponent&);
		/*
		Get a reference to the component to the right of this one
		Returns: UIComponent& - The component to the right of this one
		*/
		virtual UIComponent& getNeighbourRight();
		/*
		Set the component to the right of this one
		Parameter: UIComponent& neighbourRight - The new component to the right
		of this one
		*/
		virtual void setNeighbourRight(UIComponent&);
		/*
		Get a reference to the component below this one
		Returns: UIComponent& - The component below this one
		*/
		virtual UIComponent& getNeighbourBelow();
		/*
		Set the component below this one
		Parameter: UIComponent& neighbourBelow - The new component below this
		one
		*/
		virtual void setNeighbourBelow(UIComponent&);
		/*
		Get a reference to the component above this one
		Returns: UIComponent& - The component above this one
		*/
		virtual UIComponent& getNeighbourAbove();
		/*
		Set the component above this one
		Parameter: UIComponent& neighbourAbove - The new component above this
		one
		*/
		virtual void setNeighbourAbove(UIComponent&);
		/*
		Test whether this component is currently highlighted by a game
		controller
		Returns: bool - Whether this component is highlighted
		*/
		virtual bool isHighlighted() const;
		/*
		Set whether this component is highlighted by a game controller
		Parameter: bool highlighted - Whether this component should be
		highlighted
		*/
		virtual void setHighlighted(bool);

	protected:
		// Allow the UIGroup class to access protected memory
		friend class UIGroup;
		// This component's parent UI listener
		UIListener* m_parent = 0;
		// The ID number of this component
		unsigned int m_ID = 0;
		// Whether this component is enabled
		bool m_enabled = false;
		// Whether this component is visible
		bool m_visible = false;
		// This components neighbours
		UIComponent* m_neighbours[4] = { 0, 0, 0, 0, };
		// Whether this component is currently highlighted by a game controller
		bool m_highlighted = false;

		/*
		Draw this components graphics
		*/
		virtual void draw() = 0;
		/*
		Process user input to this component
		*/
		virtual void processInput() = 0;
		/*
		Update this component's logic
		Parameter: float - The number of frames elapsed since the last update
		*/
		virtual void update(float) = 0;
		/*
		Free this component's memory
		*/
		virtual void destroy() = 0;
	};

	// A group of UI components which can be enabled and disabled, made visible
	// and invisible, drawn, updated, and destroyed together
	class UIGroup : public UIListener {
	public:
		/*
		Initialize this UI group's memory
		Parameter: Scene& parent - The scene which controls this group
		Parameter: unsigned int ID - The ID number of this group
		Parameter: const std::string& fontFileName - The file name of the font
		with which to draw text on the components in this group
		Parameter: float textScale - The scale at which to draw text on the
		components in this group
		Parameter: const glm::vec4& textColor - The color in which to draw text
		on the components in this group
		Parameter: Controller controller - The game controller allowed to
		interact with components of this group
		Parameter: const std::string& highlightTextureFileName - The file name
		of the texture to appear behind components of this group which are
		highlighted by a game controller
		Parameter: const glm::vec2& highlightMargins - The horizontal and
		vertical margins in pixels of the highlight sprite about the background
		of components highlighted by a game controller
		Returns: bool - Whether this group could be initialized
		*/
		bool initialize(Scene&, unsigned int, const std::string&, float,
			const glm::vec4&, Controller, const std::string&, const glm::vec2&);
		/*
		Draw all the components in this group if it is visible
		*/
		void draw();
		/*
		Process user input to all the components in this group if it is enabled
		*/
		void processInput();
		/*
		A component in this group has triggered an event
		Parameter: unsigned int componentID - The ID number of the component
		Parameter: unsigned int eventID - The ID number of the event type
		*/
		void UIEvent(unsigned int, unsigned int) override;
		/*
		Update the logic, position, and dimensions of all the components in this
		group
		Parameter: float timeStep - The number of updates elapsed since the last
		update
		*/
		void update(float);
		/*
		Free this group's memory and that of all the components in it
		*/
		bool destroy();
		/*
		Get the parent scene of this group
		Returns: Scene& - This group's parent
		*/
		Scene& getParent();
		/*
		Set the parent scene of this group
		Parameter: Scene& parent - This group's new parent
		*/
		void setParent(Scene&);
		/*
		Get the ID number of this group
		Returns: unsigned int - This group's ID
		*/
		unsigned int getID() const;
		/*
		Set the ID number of this group
		Parameter: unsigned int ID - This group's new ID
		*/
		void setID(unsigned int);
		/*
		Add a component to this group
		Parameter: UIComponent& component - The component to add
		Returns: bool - Whether the component was added and was not already
		present in the group
		*/
		bool addComponent(UIComponent&);
		/*
		Remove a component from this group
		Parameter: UIComponent& component - The component to remove
		Returns: bool - Whether the component was present and was removed
		*/
		bool removeComponent(UIComponent&);
		/*
		Get the shader used to draw the components in this group
		Returns: GLSLShader& shader - This group's shader
		*/
		GLSLShader& getShader() override;
		/*
		Set the shader used to draw ths components in this group
		Parameter: const GLSLShader& shader - This group's new shader
		*/
		void setShader(const GLSLShader&) override;
		/*
		Get the file name of the font used to draw text on the components in
		this group
		Returns: const std::string& - This group's font file name
		*/
		const std::string& getFontFileName() const override;
		/*
		Set the file name of the font used to draw text on the components in
		this group
		Parameter: const std::string& fontFileName - This group's new font file
		name
		*/
		void setFontFileName(const std::string&) override;
		/*
		Get the scale at which text is drawn on the components in this group
		Returns: float - This group's text scale
		*/
		float getTextScale() const override;
		/*
		Set the scale at which text is drawn on the components in this group
		Parameter: float textScale - This group's new text scale
		*/
		void setTextScale(float) override;
		/*
		Get the color in which text is drawn on the components in this group
		Returns: const glm::vec4& - This group's text color
		*/
		const glm::vec4& getTextColor() const override;
		/*
		Set the color in which text is drawn on the components in this group
		Parameter: const glm::vec4& textColor - This group's new text color
		*/
		void setTextColor(const glm::vec4&) override;
		/*
		Test whether this group's components are enabled
		Returns: bool - Whether this group is enabled
		*/
		bool isEnabled() const;
		/*
		Set whether this group's components are enabled
		Parameter: bool enabled - Whether this enable this group
		*/
		void setEnabled(bool);
		/*
		Test whether this group's components are visible
		Returns: bool - Whether this group is visible
		*/
		bool isVisible() const;
		/*
		Set whether this group's components are visible
		Parameter: bool visible - Whether to make this group visible
		*/
		void setVisible(bool);
		/*
		Get the game controller allowed to interact with components of this
		group
		Returns: Controller - This group's game controller
		*/
		Controller getController() const override;
		/*
		Set the game controller allowed to interact with components of this
		group
		Parameter: Controller controller - This group's new game controller
		*/
		void setController(Controller) override;
		/*
		Get the component to be highlighted initially on the first input from
		this group's game controller
		Returns: UIComponent& - This group's initial component
		*/
		UIComponent& getInitialComponent();
		/*
		Set the component to be highlighted initially on the first input from
		this group's game controller
		Parameter: UIComponent& initialComponent - This group's new initial
		component
		*/
		void setInitialComponent(UIComponent&);
		/*
		Get the sprite appearing behind components highlighted by game
		controllers
		Returns: Sprite& - The highlight sprite
		*/
		Sprite& getHighlightSprite();
		/*
		Set the sprite to appear behind components highlighted by game
		controllers
		Parameter: const Sprite& highlightSprite - This group's new highlight
		sprite
		*/
		void setHighlightSprite(const Sprite&);
		/*
		Get the horizontal and vertical margins of the highlight sprite about
		components' backgrounds
		Returns: const glm::vec2& - The highlight margins
		*/
		const glm::vec2& getHighlightMargins() const;
		/*
		Set the horizontal and vertical margins of the highlight sprite about
		components' backgrounds
		Parameter: const glm::vec2& highlightMargins - The new highlight
		margins
		*/
		void setHighlightMargins(const glm::vec2&);

	private:
		// Whether this group has been initialized
		bool m_initialized = false;
		// This group's parent scene to pass UI events to
		Scene* m_parent = 0;
		// This group's ID number
		unsigned int m_ID = 0;
		// The set of components in this group
		std::vector<UIComponent*> m_components;
		// The next ID to assign to an added component
		unsigned int m_nextID = 0;
		// The shader with which to draw components in this group
		GLSLShader m_shader;
		// The file name of the font with which to draw text on the components
		// in this group
		std::string m_fontFileName = "";
		// The scale at which to draw text on the components in this group
		float m_textScale = 0.0f;
		// The color in which to draw text on the components in this group
		glm::vec4 m_textColor = glm::vec4();
		// Whether this group's components are enabled
		bool m_enabled = false;
		// Whether this group's components are visible
		bool m_visible = false;
		// Stored window dimensions to test for changes
		glm::ivec2 m_lastWindowDimensions = glm::ivec2();
		// The game controller allowed to interact with this UI group
		Controller m_controller = CONTROLLER_UNKNOWN;
		// The component of this group to be highlighted initially on the first
		// input from the game controller
		UIComponent* m_initialComponent = 0;
		// The sprite used to highlight components with game controllers
		Sprite m_highlightSprite;
		// The margins of the highlight sprite
		glm::vec2 m_highlightMargins = glm::vec2();
	};
}

#endif