#include <core/mirage.h>
#include <core/json.h>
#include <boost/json/src.hpp>
#include "config.h"
#include "server.h"
#include "client.h"

void readConfig(void)
{
	auto&& jv = mirage::parseJson(mirage::serverConfigFile);
	mirage::serverConfig = boost::json::value_to<mirage::ServerConfig>(jv);
	//mirage::onConfigRead();
}

int main(void)
{
	// readConfig();
	fmtlog::setLogLevel(fmtlog::DBG);
	fmtlog::startPollingThread();	
	mirage::network::server::networkController().start();
	mirage::ioContext().run();
	for(;;);
	return mirage::version;
}
