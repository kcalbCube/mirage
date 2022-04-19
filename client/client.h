#pragma once
#include <core/network.h>
#include <core/mirage.h>
#include <core/signal.h>

namespace mirage::network::client
{
	class Client
	{
		std::string username;
		boost::asio::ip::udp::endpoint connected;
		boost::asio::io_context context;
		boost::asio::ip::udp::socket socket;
		union
		{
			Packet<ZERO> packet;
			uint8_t data[maxPacketSize]{};
		};

		void handleReceiveFrom(
				const boost::system::error_code& ec,
				size_t size);
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
}

