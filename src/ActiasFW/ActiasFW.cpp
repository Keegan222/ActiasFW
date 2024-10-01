/*
File:		ActiasFW.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.05@14:25
Purpose:	Define variables and implement functions found in ActiasFW.h
*/

#include <Windows.h>

#include "ActiasFW.h"

namespace ActiasFW {
	// Implement Scene class functions

	bool Scene::isInitialized() const {
		return m_initialized;
	}

	bool Scene::initialize() {
		ACTIASFW_LOG(Scene, Info, "Initializing scene ", this);
		return true;
	}

	void Scene::enter(Scene& lastScene) {
		ACTIASFW_LOG(Scene, Info, "Entering scene ", this, " from scene ",
			&lastScene);
	}

	void Scene::draw() {

	}

	bool Scene::processInput() {
		return true;
	}

	void Scene::UIEvent(unsigned int groupID, unsigned int componentID,
		unsigned int eventID) {
		ACTIASFW_LOG(Scene, Info, "UI event ", eventID, " on component ",
			componentID, " of group ", groupID);
	}

	void Scene::update(float) {

	}

	void Scene::leave(Scene& nextScene) {
		ACTIASFW_LOG(Scene, Info, "Leaving scene ", this, " for scene ",
			&nextScene);
	}

	bool Scene::destroy() {
		ACTIASFW_LOG(Scene, Info, "Destroying scene ", this);
		return true;
	}

	bool Scene::initializeScene() {
		if (!m_initialized) {
			m_initialized = initialize();
		}
		return m_initialized;
	}

	bool Scene::destroyScene() {
		if (m_initialized) {
			m_initialized = !destroy();
		}
		return !m_initialized;
	}

	// Implement Application Configuration class functions

	Application::Configuration::Configuration(Scene& initialScene)
		: initialScene(initialScene) {

	}

	// Initialize Application class public static variables

	LogManager Application::Log;
	WindowManager Application::Window;
	InputManager Application::Input;
	AudioManager Application::Audio;
	GraphicsManager Application::Graphics;

	// Implement Application class functions

	bool Application::Initialize(const Configuration& configuration) {
		if (!Log.initialize(configuration.log.enabled,
			configuration.log.outputFileNames,
			configuration.log.timestampFormat)) {
			return false;
		}
		ACTIASFW_LOG(Application, Info, "Initializing Actias application");
		ACTIASFW_LOG(Application, Info, "Starting log");
		ACTIASFW_LOG(Application, Info, "Initializing Actias application ",
			"window");
		if (!Window.initialize(configuration.window.dimensions,
			configuration.window.title, configuration.window.fullscreen,
			configuration.window.targetUpdatesPerSecond,
			configuration.window.maximumUpdatesPerFrame)) {
			ACTIASFW_LOG(Application, Error, "Failed to initialize window");
			return false;
		}
		ACTIASFW_LOG(Application, Info, "Initializing user input manager");
		if (!Input.initialize()) {
			ACTIASFW_LOG(Application, Error,
				"Failed to initialize input manager");
			return false;
		}
		ACTIASFW_LOG(Application, Info, "Initializing audio manager");
		if (!Audio.initialize(configuration.audio.effectVolume,
			configuration.audio.musicVolume)) {
			ACTIASFW_LOG(Application, Error, "Failed to initialize audio ",
				"manager");
			return false;
		}
		ACTIASFW_LOG(Application, Info, "Initializing graphics manager");
		if (!Graphics.initialize(configuration.graphics.backgroundColor)) {
			ACTIASFW_LOG(Application, Error, "Failed to initialize graphics ",
				"manager");
			return false;
		}
		ACTIASFW_LOG(Application, Info, "Setting initial scene");
		SetCurrentScene(configuration.initialScene);
		return true;
	}

	void Application::Run() {
		ACTIASFW_LOG(Application, Info, "Running main application loop");
		float start = (float)glfwGetTime();
		float now = (float)glfwGetTime();
		float elapsed = 0.0f;
		float timeStep = 0.0f;
		unsigned int updates = 0;
		while (Window.update()) {
			Sleep(1);
			Graphics.begin();
			CurrentScene->draw();
			Graphics.end();
			Input.update();
			if (!CurrentScene->processInput()) {
				break;
			}
			now = (float)glfwGetTime();
			elapsed = now - start;
			start = now;
			timeStep = elapsed * (float)Window.getTargetUpdatesPerSecond();
			while (timeStep > 1.0f
				&& updates < Window.getMaximumUpdatesPerFrame()) {
				CurrentScene->update(1.0f);
				Graphics.update(1.0f);
				timeStep -= 1.0f;
				updates++;
			}
			CurrentScene->update(timeStep);
			Graphics.update(timeStep);
			updates = 0;
		}
	}

	bool Application::InitializeScene(Scene& scene) {
		ACTIASFW_LOG(Application, Info, "Attempting to initialize scene ",
			&scene);
		if (std::find(Scenes.begin(), Scenes.end(), &scene) == Scenes.end()) {
			ACTIASFW_LOG(Application, Info, "Adding scene ", &scene,
				" to memory");
			Scenes.push_back(&scene);
		}
		else {
			ACTIASFW_LOG(Application, Info, "Scene ", &scene,
				" is already in memory");
			return false;
		}
		return scene.initializeScene();
	}

	void Application::SetCurrentScene(Scene& nextScene) {
		ACTIASFW_LOG(Application, Info, "Setting new scene ", &nextScene);
		if (CurrentScene != 0) {
			ACTIASFW_LOG(Application, Info, "Leaving old scene ", CurrentScene);
			Input.removeListener(*CurrentScene);
			CurrentScene->leave(nextScene);
		}
		if (&nextScene == CurrentScene) {
			ACTIASFW_LOG(Application, Warning, "New scene is the current ",
				"scene");
			CurrentScene = 0;
			return;
		}
		if (InitializeScene(nextScene) || nextScene.isInitialized()) {
			ACTIASFW_LOG(Application, Info, "Entering new scene ", &nextScene);
			if (CurrentScene != 0) {
				nextScene.enter(*CurrentScene);
			}
			else {
				nextScene.enter(nextScene);
			}
			CurrentScene = &nextScene;
			Input.addListener(*CurrentScene);
		}
	}

	bool Application::DestroyScene(Scene& scene) {
		ACTIASFW_LOG(Application, Info, "Attempting to destroy scene ", &scene);
		if (&scene == CurrentScene) {
			ACTIASFW_LOG(Application, Warning, "Scene ", &scene,
				" is the current scene");
			return false;
		}
		std::vector<Scene*>::iterator it = std::find(Scenes.begin(),
			Scenes.end(), &scene);
		if (it == Scenes.end()) {
			ACTIASFW_LOG(Application, Warning, "Scene ", &scene,
				" not found in memory");
			return false;
		}
		Scenes.erase(it);
		scene.destroyScene();
		return true;
	}

	bool Application::Destroy() {
		ACTIASFW_LOG(Application, Info, "Destroying Actias application");
		bool success = true;
		ACTIASFW_LOG(Application, Info, "Destroying all scenes");
		SetCurrentScene(*CurrentScene);
		for (Scene* scene : Scenes) {
			if (!(success = scene->destroyScene())) {
				ACTIASFW_LOG(Application, Warning, "Failed to destroy scene ",
					scene);
			}
		}
		ACTIASFW_LOG(Application, Info, "Destroying graphics manager");
		if (!(success == Graphics.destroy())) {
			ACTIASFW_LOG(Application, Warning, "Failed to destroy graphics ",
				"manager");
		}
		ACTIASFW_LOG(Application, Info, "Destroying audio manager");
		if (!(success = Audio.destroy())) {
			ACTIASFW_LOG(Application, Warning, "Failed to destroy audio ",
				"manager");
		}
		ACTIASFW_LOG(Application, Info, "Destroying user input manager");
		if (!(success = Input.destroy())) {
			ACTIASFW_LOG(Application, Warning, "Failed to destroy input ",
				"manager");
		}
		ACTIASFW_LOG(Application, Info, "Destroying window");
		if (!(success = Window.destroy())) {
			ACTIASFW_LOG(Application, Warning, "Failed to destroy window");
		}
		ACTIASFW_LOG(Application, Info, "Stopping log");
		if (!(success = Log.destroy())) {
			ACTIASFW_LOG(Application, Warning, "Failed to destroy log");
		}
		return success;
	}

	// Initialize Application class private static variables

	std::vector<Scene*> Application::Scenes;
	Scene* Application::CurrentScene = 0;
}