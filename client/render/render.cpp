#include "render.h"

void mirage::client::GameRenderer::render(MainWindowUpdateEvent&)
{
	SDL_SetRenderDrawColor(mainWindow().renderer, 255, 0, 0, 255);
	for(auto&& vertice : frame)
	{
		SDL_Rect rect{int(vertice.x * 200.f), int(vertice.y * 200.f), 10, 20};
		SDL_RenderDrawRect(mainWindow().renderer, &rect);	
	}
}

void mirage::client::GameRenderer::onFrame(mirage::network::client::PacketReceivedEvent<network::GraphicFrame>& packet)
{
	frame = utils::deserialize<decltype(frame)>
		(std::string{utils::stringView(packet.packet.serialized, std::size(packet.packet.serialized))});
}

void mirage::client::GameRenderer::initialize(void) {}
void mirage::client::GameRenderer::lateInitialize(void)
{
	bindEvent<MainWindowUpdateEvent, &GameRenderer::render>();
	bindEvent<network::client::PacketReceivedEvent<network::GraphicFrame>, &GameRenderer::onFrame>();
}
