#include "client.h"

int main(void)
{
	mirage::network::client::Client client("kcalbcube");
	client.connect(mirage::network::fromString("127.0.0.1", 5000));
	
	return 0;
}
