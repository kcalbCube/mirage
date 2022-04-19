#pragma once
#include <cstdint>
#include <core/mirage.h>
#include <memory>
#include <boost/asio/buffer.hpp>
#include <type_traits>
#include <boost/asio/ip/udp.hpp>
#include <boost/asio/placeholders.hpp>

namespace mirage::network
{
	constexpr uint32_t packetConstant = 0x4752494D;
	constexpr size_t maxPacketSize = 1024;
	constexpr size_t usernameMax = 32;

	enum PacketId : uint8_t
	{
		ZERO,
		TIMEOUT,
		/* c->s InitializeConnection
		 * s->c ConnectionResponce
		 */
		CONNECT,
		/* c->s ClientDisconnect
		 * s->c ServerDisconnect
		 */
		DISCONNECT,
	};
#pragma pack(push, 1)
	template<PacketId ID>
	struct Packet
	{
		uint32_t constant = packetConstant;
		PacketId id = ID;
	};

	struct Timeout : Packet<TIMEOUT>
	{
	};

	struct InitializeConnection : Packet<CONNECT>
	{
		char username[usernameMax]{};
	};

	struct ConnectionResponce : Packet<CONNECT>
	{
		enum : uint8_t
		{
			UNAVAILABLE,
			BANNED,
			ALREADY_CONNECTED,
			CONNECTED,
		} responce;
	};

	struct ClientDisconnect : Packet<DISCONNECT>
	{
	};

	struct ServerDisconnect : Packet<DISCONNECT>
	{
	};
#pragma pack(pop)

	inline boost::asio::ip::udp::endpoint fromString(const std::string& sv, int port)
	{
		boost::system::error_code ec;
		boost::asio::ip::address address = 
			boost::asio::ip::address::from_string(sv, ec);
		if(ec.value() != 0)
		{
			// FIXME:
			abort();
		}

		return boost::asio::ip::udp::endpoint(address, port);

	}

	// a view to a packet
	struct AbstractPacket
	{
		const Packet<ZERO>* packet = nullptr;
		size_t size = 0;

		template<typename T>
		inline AbstractPacket(T& packet_)
		{
			packet = reinterpret_cast<const Packet<ZERO>*>(&packet_);
			size = sizeof(std::decay_t<T>);
		}
		inline AbstractPacket(const Packet<ZERO>* const packet_, size_t size_)
			: packet{packet_}, size{size_} {}
		AbstractPacket(auto&&) = delete;


		inline operator boost::asio::const_buffer(void) const
		{
			return boost::asio::const_buffer(packet, size);
		}
	};

	template<typename T>
	inline const T& packetCast(const AbstractPacket& packet)
	{
		if(!packet.packet)
			; // FIXME:
		return reinterpret_cast<const T&>(*packet.packet);
	}
}
