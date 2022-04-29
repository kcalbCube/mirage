#pragma once
#include <boost/json.hpp>
#include <core/signal.h>

namespace mirage
{
	inline const auto serverConfigFile = "config.json";
	
	inline struct ServerConfig
	{
		int port = 5000;
		std::string serverName = "FILLME";
	} serverConfig{};

	
	ServerConfig tag_invoke(boost::json::value_to_tag<ServerConfig>, boost::json::value const& jv)
	{
		auto&& obj = jv.as_object();
		return ServerConfig 
		{
			boost::json::value_to<int>(obj.at("server_port")),
			boost::json::value_to<std::string>(obj.at("server_name"))
		};
	}

	//inline boost::signals2::signal<void(void)> onConfigRead;
}
