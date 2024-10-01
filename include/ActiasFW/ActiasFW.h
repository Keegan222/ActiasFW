/*
File:		ActiasFW.h
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.05@14:24
Purpose:	Contains the main scene and applications classes for the Actias
			Framework
*/

#ifndef ACTIASFW_ACTIASFW_H
#define ACTIASFW_ACTIASFW_H

#include "Audio.h"
#include "Logging.h"
#include "UIComponents.h"
#include "Windowing.h"

namespace ActiasFW {
	// An abstract scene run by an ActiasFW application
	class Scene : public InputListener {
	public:
		/*
		Test whether this scene has been initialized
		Returns: bool - Whether this scene is initialized
		*/
		bool isInitialized() const;

	protected:
		// Allow UIGroups to access protected memory
		friend class UIGroup;

		/*
		Initialize this scene's memory
		Returns: bool - Whether the scene was initialized successfully
		*/
		virtual bool initialize();
		/*
		Enter this scene from another scene
		Parameter: Scene& lastScene - The last scene in the application
		*/
		virtual void enter(Scene&);
		/*
		Draw this scene's graphics
		*/
		virtual void draw();
		/*
		Process user input to this scene
		Returns: bool - Whether to continue the application running this scene
		*/
		virtual bool processInput();
		/*
		Process a UI component even on this scene
		Parameter: unsigned int groupID - The source UI group of this event
		Parameter: unsigned int componentID - The source UI component of this
		event
		Parameter: unsigned int eventID - The event type
		*/
		virtual void UIEvent(unsigned int, unsigned int, unsigned int);
		/*
		Update this scene's logic
		Parameter: float timeStep - The number of frames elapsed since the last
		call to update
		*/
		virtual void update(float);
		/*
		Leave this scene for another scene
		Parameter: Scene& nextScene - The next scene in the application
		*/
		virtual void leave(Scene&);
		/*
		Free this scene's memory
		Returns: bool - Whether this scene was destroyed successfully
		*/
		virtual bool destroy();

	private:
		// Allow the Application class to access private memory
		friend class Application;

		// Whether this scene has been initialized
		bool m_initialized = false;

		/*
		Call the initialize function and set the initialized flag
		Returns: bool - Whether this scene was initialized successfully
		*/
		bool initializeScene();
		/*
		Call the destroy function and set the initialized flag
		Returns: bool - Whether this scene was destroyed successfully
		*/
		bool destroyScene();
	};

	// The central class of the ActiasFW framework, an application with all the
	// functionality of the framework
	class Application {
	public:
		// The configuration structure for ActiasFW applications
		struct Configuration {
			// The log configuration structure for ActiasFW applications
			struct Log {
				// Whether the logging system is initially enabled
#ifdef _DEBUG
				bool enabled = true;
#else
				bool enabled = false;
#endif
				// The initial list of file names to write logs to
#ifdef _DEBUG
				std::vector<std::string> outputFileNames = { "ActiasFW.log" };
#else
				std::vector<std::string> outputFileNames = {};
#endif
				// The initial format to write timestamps in logs in
				std::string timestampFormat = "%Y.%m.%d@%H:%M:%S";
			} log;
			// The window configuration structure for ActiasFW applications
			struct Window {
				// The initial windowed dimensions of the window
				glm::ivec2 dimensions = glm::ivec2(800, 600);
				// The initial title of the window
				std::string title = "ActiasFW";
				// Whether the window initially appears in fullscreen mode
				bool fullscreen = false;
				// The target number of logic updates per second
				unsigned int targetUpdatesPerSecond = 60;
				// The maximum number of logic updates per frame of the window
				unsigned int maximumUpdatesPerFrame = 10;
			} window;
			// The audio configuration structure for ActiasFW applications
			struct Audio {
				// The initial sound effect volume (0.0 to 1.0)
				float effectVolume = 1.0f;
				// The initial music volume (0.0 to 1.0)
				float musicVolume = 1.0f;
			} audio;
			// The graphics configuration structure for ActiasFW applications
			struct Graphics {
				// The background color of the window
				glm::vec3 backgroundColor = glm::vec3(0.0f, 0.0f, 0.0f);
			} graphics;
			// The initial scene to set in the application
			Scene& initialScene;

			/*
			Construct an ActiasFW application configuration
			Parameter: Scene& initialScene - The initial scene to set in the
			application
			*/
			Configuration(Scene&);
		};

		// This application's log manager
		static LogManager Log;
		// This application's window manager
		static WindowManager Window;
		// This application's input manager
		static InputManager Input;
		// This application's audio manager
		static AudioManager Audio;
		// This application's graphics manager
		static GraphicsManager Graphics;

		/*
		Initialize the application
		Parameter: const Configuration& configuration - The configuration
		structure to use to set up the applications functionality
		Returns: bool - Whether the application was initialized successfully
		*/
		static bool Initialize(const Configuration&);
		/*
		Run the application's main loop until it is stopped by its
		current scene
		*/
		static void Run();
		/*
		Initialize a scene and add it to the application's memory
		Parameter: Scene& scene - The scene to be initialized
		Returns: bool - Whether the scene was not already in memory and was
		initialized successfully
		*/
		static bool InitializeScene(Scene&);
		/*
		Change the current scene running in the application
		Parameter: Scene& nextScene - The new scene to run in the application
		*/
		static void SetCurrentScene(Scene&);
		/*
		Destroy a scene and remove it from the application's memory
		Parameter: Scene& scene - The scene to be destroyed (must not be the
		currently running scene)
		Returns: bool - The the scene was in memory, was not the current scene,
		and was destroyed successfully
		*/
		static bool DestroyScene(Scene&);
		/*
		Free the application's memory
		Returns: bool - Whether the application was successfully destroyed
		*/
		static bool Destroy();

	private:
		// The set of scenes currently in the application's memory
		static std::vector<Scene*> Scenes;
		// The current scene running in the application
		static Scene* CurrentScene;
	};
}

#endif