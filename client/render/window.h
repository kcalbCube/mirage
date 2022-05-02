#pragma once
#include <SDL2/SDL.h>
#include <core/utility.h>
#include <core/ecs.h>

namespace mirage::client
{
	class MainWindow
	{
	public:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		void render(void);
	
		void initialize(unsigned w, unsigned h);	
		void deinitialize(void);

		void handleEvents(void);

		~MainWindow(void) { deinitialize(); }
	};

	MIRAGE_COFU(MainWindow, mainWindow);

	struct MainWindowUpdateEvent {};
}
