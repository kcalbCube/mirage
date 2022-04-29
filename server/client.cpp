#include "client.h"
#include "core/ecs.h"
#include "core/network.h"
#include "server/server.h"
#include <boost/bind.hpp>
namespace mirage::server
{
	bool Client::isAuthorized(void) const
	{
		return authorized;
	}

	const network::server::Connection& Client::getConnection(void) const
	{
		return connection;
	}

	std::string_view Client::getUsername(void) const
	{
		return connection.username;
	}

	void Client::staticInitialize(void)
	{
	}

	void Client::onDestroy(void)
	{
		network::server::networkController().disconnect(connection);
	}

	void Client::authorizationBlocked(const ClientAuthorizationBlockedEvent& event)
	{
		if(!authorized && event.username == getUsername())
			destroy();
	}

	void Client::authorizationConfirmed(const ClientAuthorizationConfirmedEvent& event)
	{
		if(authorized || event.username != getUsername())
			return;

		authorized = true;
		event::emitter().publish<ClientAuthorizedEvent>(entity);
	}

	void Client::initialize(const network::server::NewConnectionEvent& event)
	{
		bindEvent<ClientAuthorizationBlockedEvent>  (&Client::authorizationBlocked);
		bindEvent<ClientAuthorizationConfirmedEvent>(&Client::authorizationConfirmed);

		event::emitter().publish<ClientAuthorizationRequestEvent>(entity);
	}

	void Client::sendMessage(std::string_view str)
	{
		network::MessageSent msg;
		memcpy(msg.message, str.data(), str.size());
		
		network::server::networkController().send(
			getConnection(), 
			network::AbstractPacket(msg));
	}

}
