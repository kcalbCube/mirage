#include "server.h"
#include <ranges>
#include <algorithm>
#include <core/utility.h>
#include <iostream>
#include <string_view>
#include <algorithm>

namespace mirage::network::server
{
	NetworkController::NetworkController(unsigned short port_)
		: port{port_}, 
		  socket(context, 
			boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	{
	}

	unsigned short NetworkController::getPort(void) const 
	{ 
		return port; 
	}

	void NetworkController::handleConnect(
			const boost::asio::ip::udp::endpoint& endpoint,
			const InitializeConnection& packet)
	{
		auto username = mirage::utils::sanitizeUsername(
				std::string(std::string_view(packet.username, 
				usernameMax)));

		if(auto&& con = getConnection(username); con.isValid())
		{
			logi("{} connection refused: already connected", con.username);
			ConnectionResponce cr {.responce = ConnectionResponce::ALREADY_CONNECTED};
			send(con, AbstractPacket(cr));
			return;
		}
		auto& con = connections.emplace_back(endpoint, username);

		const auto& ccon = con;
		ConnectionResponce cr;

		bool shouldErase = false;
		if(newConnectionUnavailable(ccon))
		{
			logi("{} connection refused: unavailable", con.username);
			cr.responce = ConnectionResponce::UNAVAILABLE;
			shouldErase = true;
		}
		else if(newConnectionBanned(ccon))
		{
			logi("{} connection refused: banned", con.username);
			cr.responce = ConnectionResponce::BANNED;
			shouldErase = true;
		}
		else
		{
			logi("{} connected", con.username);
			cr.responce = ConnectionResponce::CONNECTED;
		}

		if(shouldErase)
			disconnectForce(ccon);	
		else
			onNewConnection(con);

		send(con, AbstractPacket(cr));
			
	}

	void NetworkController::handlePacketRaw(
			const boost::asio::ip::udp::endpoint& endpoint,
			const AbstractPacket& packet)
	{
		logi("received packet, c {}, id {}", packet.packet->constant, packet.packet->id);
		if(packet.packet->constant != packetConstant)
			return;

		switch(packet.packet->id)
		{
			case CONNECT:
				handleConnect(endpoint, 
					packetCast<InitializeConnection>(packet));
				break;
			default:
			{
				const auto connection = getConnection(endpoint);
				if(connection.isValid())
					handlePacket(connection, packet);
			}
		};
	}
	void NetworkController::handleReceiveFrom(
			const boost::system::error_code& ec, 
			size_t size)
	{
		if(ec && ec != boost::asio::error::message_size)
			return;
		handlePacketRaw(endpoint, AbstractPacket(&packet, size));
		startReceive();
	}

	void NetworkController::disconnectForce(const Connection& connection)
	{
		std::erase_if(connections,
				[&connection](const auto& a) -> bool
				{
					return &a == &connection;
				});
	}

	const Connection& NetworkController::getConnection(std::string_view username) const
	{
		const auto&& it = std::ranges::find_if(connections,
				[&username](auto&& a) -> bool
				{
					return a.username == username;
				});
		if(it == connections.end())
			return invalidConnection;
		return *it;

	}

	const Connection& NetworkController::getConnection(
			const boost::asio::ip::udp::endpoint& endpoint) const
	{
		const auto&& it = std::ranges::find_if(connections,
				[&endpoint](auto&& a) -> bool
				{
					return a.endpoint == endpoint;
				});
		if(it == connections.end())
			return invalidConnection;
		return *it;
	}

	void NetworkController::startReceive(void)
	{
		socket.async_receive_from(
				boost::asio::buffer(data), endpoint,
				boost::bind(&NetworkController::handleReceiveFrom,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
	}

	void NetworkController::start(void)
	{
		startReceive();
		context.run();
	}

	void NetworkController::send(
			const Connection& connection,
			const boost::asio::const_buffer& buffer)
	{
		socket.send_to(buffer, connection.endpoint);
	}
}
