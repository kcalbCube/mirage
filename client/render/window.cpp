#include "window.h"
#include <backends/imgui_impl_sdlrenderer.h>
#include <backends/imgui_impl_sdl.h>
#include <core/ecs.h>
#include <core/mirage.h>
#include <SDL.h>
#include <core/logging.h>
#include <imgui.h>

void mirage::client::MainWindow::initialize(unsigned w, unsigned h)
{
	if(SDL_Init(SDL_INIT_EVERYTHING))
	{
		logi("Unable initialize SDL2. Aborting.");
		abort();
	}

	window = SDL_CreateWindow(fmt::format("mirage {}", mirage::version).c_str(),
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		w,
		h,
		SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE
	);

	renderer = SDL_CreateRenderer(window, -1, 
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
	);

	if(!renderer)
	{
		logi("Unable intialize video. Aborting.");
		abort();
	}

	SDL_RendererInfo info;
	SDL_GetRendererInfo(renderer, &info);
	logi("Current SDL_Renderer: {}", info.name);

	IMGUI_CHECKVERSION();

	ImGui::CreateContext();

	(void)ImGui::GetIO();

	ImGui::StyleColorsDark();

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer_Init(renderer);
}

void mirage::client::MainWindow::handleEvents(void)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);	
	}
}
void mirage::client::MainWindow::render(void)
{
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);    
        SDL_SetRenderDrawColor(renderer, 
		(Uint8)(clear_color.x * 255), 
		(Uint8)(clear_color.y * 255), 
		(Uint8)(clear_color.z * 255), 
		(Uint8)(clear_color.w * 255));
        SDL_RenderClear(renderer);

	event::triggerEvent<MainWindowRenderEvent>();
	ImGui_ImplSDLRenderer_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	
	event::triggerEvent<MainWindowUpdateEvent>();

	ImGui::Render();	

        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(renderer);
}

void mirage::client::MainWindow::deinitialize(void)
{
	ImGui_ImplSDLRenderer_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}


