#include "client.h"
#include <core/network.h>
#include <iostream>
#include <core/event.h>
#include <thread>
#include "render/window.h"
#include <imgui.h>

struct MessageWindow : mirage::ecs::Component<MessageWindow>
{
	std::string message;
	int i = 0;

	void windowUpdate(mirage::client::MainWindowUpdateEvent)
	{
		ImGui::Begin(fmt::format("Message #{}", i).c_str());
		ImGui::Text(message.c_str());
		ImGui::End();
	}

	void initialize(mirage::network::client::PacketReceivedEvent packet)
	{
		if(packet.packet.packet->id != mirage::network::PacketId::message)
		{
			destroy();
			return;
		}

		static int counter = 0;

		auto msg = mirage::network::packetCast<mirage::network::MessageSent>(packet.packet);
		message = std::string_view(msg.message, msg.messageMax);
		i = counter++;
		bindEvent<mirage::client::MainWindowUpdateEvent>(&MessageWindow::windowUpdate);
	}
};

MIRAGE_CREATE_WITH_EVENT(mirage::network::client::PacketReceivedEvent, MessageWindow);
int main(void)
{
	fmtlog::setLogLevel(fmtlog::DBG);
 	fmtlog::startPollingThread();		
	mirage::network::client::Client client("kcalbcube");	
	client.start();	
	client.connect(mirage::network::fromString("127.0.0.1", 5000));	

	std::thread thr([](void) -> void
	{
		mirage::client::mainWindow().initialize(600, 300);

		while(true)
		{
			mirage::client::mainWindow().render();
			mirage::client::mainWindow().handleEvents();
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}
	});

	thr.detach();

	mirage::ioContext().run();
	while(true) ;
	return 0;
}
