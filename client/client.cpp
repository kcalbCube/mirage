#include "client.h"
#include "core/network.h"

namespace mirage::network::client
{
	void Client::handleReceiveFrom(
			const boost::system::error_code& ec,
			size_t size)
	{

	}

	Client::Client(std::string sv)
		: username {std::move(sv)}, 
		  socket(context,
			boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)) {}
	
	Client::~Client(void) {}
	void Client::connect(boost::asio::ip::udp::endpoint con)
	{
		connected = std::move(con);

		InitializeConnection incon;
		strcpy(incon.username, username.c_str());
		
		send(AbstractPacket(incon));
	}

	void Client::send(const boost::asio::const_buffer& buffer)
	{
		socket.send_to(buffer, connected);
	}

	void Client::startReceive(void)
	{
		socket.async_receive_from(
				boost::asio::buffer(data), connected,
				boost::bind(&Client::handleReceiveFrom,
					this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
	}

	void Client::start(void)
	{
		startReceive();
		context.run();
	}
}
