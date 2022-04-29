#pragma once
#include "server/client.h"

namespace game
{
	class ClientAuthorization : public mirage::ecs::Component<ClientAuthorization>
	{	
	public:
		mirage::ecs::ComponentWrapper<mirage::server::Client> client;
		struct AuthorizationProcess : Process<AuthorizationProcess, unsigned>
		{
			int counter = 0;
			void update(unsigned delta, void*);
			virtual void failed(void) override;
			virtual void succeeded(void) override;
		};

		void initialize(entt::entity, 
				mirage::server::ClientAuthorizationRequestEvent&);	
	};
	MIRAGE_CREATE_WITH_EVENT(
		mirage::server::ClientAuthorizationRequestEvent, 
		ClientAuthorization);
}
