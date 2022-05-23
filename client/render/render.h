#pragma once
#include <SDL.h>
#include <core/graphics.h>
#include <core/network.h>
#include "window.h"
#include <core/event.h>
#include <core/ecs.h>
#include <vector>
#include "../client.h"

namespace mirage::client
{
	class GameRenderer
		: public ecs::Component<GameRenderer>
	{
	public:
		std::vector<graphics::Vertice> frame;
		void render(MainWindowUpdateEvent&);

		void onFrame(
			network::client::PacketReceivedEvent<network::GraphicFrame>&);	

		void initialize(void);
		void lateInitialize(void);
	};
	MIRAGE_CREATE_ON_STARTUP(GameRenderer, gameRenderer);
}
