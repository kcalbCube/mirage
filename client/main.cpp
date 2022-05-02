#include "client.h"
#include <core/network.h>
#include <iostream>
#include <core/event.h>

int main(void)
{
	fmtlog::setLogLevel(fmtlog::DBG);
 	fmtlog::startPollingThread();		
	mirage::network::client::Client client("kcalbcube");	
	client.start();	
	mirage::event::emitter().on<mirage::network::client::PacketReceivedEvent>(
		[](mirage::network::client::PacketReceivedEvent& packet, 
			mirage::event::Emitter&)->void
		{
			if(packet.packet.packet->id == mirage::network::PacketId::message)
			{
				auto msg = mirage::network::packetCast<
					mirage::network::MessageSent>
					(packet.packet);
				std::cout << std::string_view(msg.message, msg.messageMax)
					<< std::endl;
			}
		});
	client.connect(mirage::network::fromString("127.0.0.1", 5000));	
	mirage::ioContext().run();
	while(true) ;
	return 0;
}
