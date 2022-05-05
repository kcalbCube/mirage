#include "client.h"

void game::ClientAuthorization::AuthorizationProcess::update(unsigned int delta, void*)
{
	if(!parent || !parent->client)
	{
		fail();
		return;
	}
	if(counter++ > 10)
	{
		fail();
		return;
	}
	if(counter % 2)
	{
		parent->client.get().sendMessage("Enter password!");
	}
}

void game::ClientAuthorization::AuthorizationProcess::failed(void)
{
	if(parent && parent->client)
	{
		parent->client->sendMessage("Authorization failed!");
		parent->destroy();
	}
}

void game::ClientAuthorization::onPacket(
		mirage::network::server::PacketReceivedEvent<mirage::network::MessageSent>& packet)
{	
	if(packet.username != client->getUsername())
		return;

	if(packet.packet.view() == "kcalbCubinho")
	{
		client->sendMessage("Authorized!");
		mirage::event::emitter().publish<mirage::server::ClientAuthorizationConfirmedEvent>
			(std::string(client->getUsername()));
		destroy();
	}
	else
		client->sendMessage("Wrong password!");
}
void game::ClientAuthorization::initialize( 
		mirage::server::ClientAuthorizationRequestEvent& request)
{
	startProcess<AuthorizationProcess>(
			mirage::ecs::processing::PeriodMS<1000>::getInstance(), 
			entity);
	bindEvent<mirage::network::server::PacketReceivedEvent<mirage::network::MessageSent>>(
			&ClientAuthorization::onPacket);
	client = request.client;
}
