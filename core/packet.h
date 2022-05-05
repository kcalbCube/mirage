/*
 * forward version of network.h
 */
#pragma once
#include <string>
#include "utility.h"
#include <boost/asio/ip/udp.hpp>

namespace mirage::network
{
	constexpr uint32_t packetConstant = 0x4752494D;
	constexpr size_t maxPacketSize = 1024;
	constexpr size_t usernameMax = 32;

	enum class PacketId : uint8_t
	{
		zero,
		timeout,
		/* c->s InitializeConnection
		 * s->c ConnectionResponce
		 */
		connect,
		/* c->s ClientDisconnect
		 * s->c ServerDisconnect
		 */
		disconnect,
		/* 
		 * MessageSent
		 */
		message,
		/*
		 * s->c ServerInfo
		 */
		serverInfo,
	};

#pragma pack(push, 1)
	template<PacketId ID>
	struct Packet
	{
		uint32_t constant = packetConstant;
		PacketId id = ID;
	};

	using PacketVoid = Packet<PacketId::zero>;

	struct Timeout : Packet<PacketId::timeout>
	{
	};

	struct InitializeConnection : Packet<PacketId::connect>
	{
		char username[usernameMax]{};
	};

	struct ConnectionResponce : Packet<PacketId::connect>
	{
		enum : uint8_t
		{
			unavailable,
			banned,
			alreadyConnected,
			success,
		} responce;
	};

	struct ClientDisconnect : Packet<PacketId::disconnect>
	{
	};

	struct ServerDisconnect : Packet<PacketId::disconnect>
	{
	};
	
	struct MessageSent : Packet<PacketId::message>
	{
		static constexpr size_t messageMax = 128;
		char message[messageMax]{};

		std::string_view view(void) const
		{
			return utils::stringView(message, messageMax);
		}
	};

	struct ServerInfo : Packet<PacketId::serverInfo>
	{
		static constexpr size_t infoMax = 512;
		char info[infoMax]{}; // json
	};

#pragma pack(pop)

	struct AbstractPacket;
}
