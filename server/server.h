#pragma once

#include <core/network.h>
#include <core/mirage.h>
#include <core/signal.h>
#include <core/logging.h>
#include <string>

namespace mirage::network::server
{
	class Connection
	{
	public:
		boost::asio::ip::udp::endpoint endpoint;
		std::string username;	

		inline bool isValid(void) const 
		{
			return !username.empty();
		}
		inline bool operator==(const Connection& connection) const
		{
			return &connection == this;
		}
	};

	inline Connection invalidConnection;

	class NetworkController
	{
		unsigned short port = 5000;
		std::vector<Connection> connections;
		boost::asio::io_context context;	
		boost::asio::ip::udp::endpoint endpoint;
		boost::asio::ip::udp::socket socket;
		union
		{
			Packet<ZERO> packet;
			uint8_t data[maxPacketSize]{};
		};

		void handleConnect(
				const boost::asio::ip::udp::endpoint&,
				const InitializeConnection&);
		void handlePacketRaw(
				const boost::asio::ip::udp::endpoint&,
				const AbstractPacket&);
		void handleReceiveFrom(const boost::system::error_code&, size_t);
		void startReceive(void);
	public:
		NetworkController(unsigned short port);
		
		unsigned short getPort(void) const;

		const Connection& getConnection(std::string_view username) const;
		const Connection& getConnection(const boost::asio::ip::udp::endpoint&) const;
		void disconnect(std::string_view username);
		void disconnect(const Connection&);
		/*
		 * Disconnect without replying.
		 */
		void disconnectForce(const Connection&);
	
		Signal <void(Connection&)> 				onNewConnection;
		SignalR<bool(const Connection&), SignalAny<bool, true>>  newConnectionUnavailable;
		SignalR<bool(const Connection&), SignalAny<bool, true>>  newConnectionBanned;
		Signal <int(Connection&)> 				onDisconnect;
		Signal <void(const Connection&, const AbstractPacket&)> handlePacket;

		void start(void);
		void send(const Connection&, const boost::asio::const_buffer&);
	};
}
