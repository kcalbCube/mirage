#include "client.h"
#include "server/client.h"

void game::ClientAuthorization::AuthorizationProcess::update(unsigned int delta, void*)
{
	auto* parent = getParent();
	if(!parent || !parent->client.tryGet())
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

void game::ClientAuthorization::AuthorizationProcess::succeeded(void)
{
	if(auto* client = getParent()->client.tryGet(); client)
		client->sendMessage("Authorized!");
}

void game::ClientAuthorization::AuthorizationProcess::failed(void)
{
	auto* parent = getParent();
	if(!parent || !parent->client.tryGet())
	{
		parent->client.get().sendMessage("Authorization failed!");
	}
}

void game::ClientAuthorization::initialize(
		entt::entity, 
		mirage::server::ClientAuthorizationRequestEvent& request)
{
	client = request.client;
}
