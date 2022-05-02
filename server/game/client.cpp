#include "client.h"
#include "core/processing.h"
#include "server/client.h"

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

void game::ClientAuthorization::AuthorizationProcess::succeeded(void)
{
	if(auto* client = parent->client.tryGet(); client)
		client->sendMessage("Authorized!");
}

void game::ClientAuthorization::AuthorizationProcess::failed(void)
{
	if(parent && parent->client)
	{
		parent->client.get().sendMessage("Authorization failed!");
	}
}

void game::ClientAuthorization::initialize( 
		mirage::server::ClientAuthorizationRequestEvent& request)
{
	startProcess<AuthorizationProcess>(mirage::ecs::processing::PeriodMS<1000>::getInstance(), 
			entity);
	client = request.client;
}
