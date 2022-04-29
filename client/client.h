#pragma once
#include <core/network.h>
#include <core/mirage.h>
#include <core/signal.h>

namespace mirage::network::client
{
	class Client
	{
		std::string username;
		boost::asio::ip::udp::endpoint connected, endpoint;	
		boost::asio::ip::udp::socket socket;
		union
		{
			PacketVoid packet;
			uint8_t data[maxPacketSize]{};
		};

		void handleReceiveFrom(
				const boost::system::error_code&,
				size_t);
		void handlePacketRaw(const AbstractPacket&);
	public:	

		Client(std::string);
		Client(Client&&);
		Client(const Client&) = delete;
		~Client(void);

		const boost::asio::ip::udp::endpoint& getConnected(void) const;
		void connect(boost::asio::ip::udp::endpoint);
	
		void disconnect(void);
		void send(const boost::asio::const_buffer&);

		void startReceive(void);
		void start(void);
	};

	struct PacketReceivedEvent
	{
		AbstractPacket packet;
	};
}

