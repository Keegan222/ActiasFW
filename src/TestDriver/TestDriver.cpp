/*
File:		TestDriver.cpp
Author:		Keegan MacDonald (keeganm742@gmail.com)
Created:	2024.08.05@13:58
Purpose:	Implement functions found in the TestDriver class of TestDriver.h
			and the main entry point of the TestDriver program
*/

#include "TestDriver.h"

TitleScene TestDriver::Title;
OptionsScene TestDriver::Options;
GameScene TestDriver::Game;

int main(int argc, char** argv) {
	srand((unsigned int)time(0));
	Application::Configuration configuration(TestDriver::Title);
	configuration.window.dimensions = glm::ivec2(960, 540);
	if (!Application::Initialize(configuration)) {
		std::cout << "Failed to initialize Actias application" << std::endl;
		return EXIT_FAILURE;
	}
	OptionsManager::Load();
	Application::Window.setDimensions(OptionsManager::WindowDimensions);
	Application::Window.setMonitor(OptionsManager::Monitor);
	Application::Window.setFullscreen(OptionsManager::Fullscreen);
	Application::Audio.setEffectVolume(OptionsManager::EffectVolume);
	Application::Run();
	Application::Destroy();
	return EXIT_SUCCESS;
}