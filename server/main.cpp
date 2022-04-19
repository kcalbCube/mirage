#include <core/mirage.h>
#include <core/json.h>
#include <boost/json/src.hpp>

#include "config.h"
#include "server.h"

void readConfig(void)
{
	auto&& jv = mirage::parseJson(mirage::serverConfigFile);
	mirage::serverConfig = boost::json::value_to<mirage::ServerConfig>(jv);
	mirage::onConfigRead();
}

int main(void)
{
	// readConfig();
	fmtlog::setLogLevel(fmtlog::DBG);
	fmtlog::startPollingThread();
	mirage::network::server::NetworkController nc(5000);
	nc.start();
	for(;;);
	return mirage::version;
}
