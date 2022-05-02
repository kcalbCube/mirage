#pragma once
#include "server/client.h"
#include <core/processing.h>

namespace game
{
	class ClientAuthorization : 
		public mirage::ecs::Component<ClientAuthorization>,
		public mirage::ecs::Processing<ClientAuthorization>
	{	
	public:
		mirage::ecs::ComponentWrapper<mirage::server::Client> client;
		struct AuthorizationProcess : Process<AuthorizationProcess, unsigned>
		{
			mirage::ecs::ComponentWrapper<ClientAuthorization> parent;
			AuthorizationProcess(entt::entity entity)
				: parent(entity) {}

			int counter = 0;
			void update(unsigned delta, void*);
			void failed(void);
			void succeeded(void);
		};

		void initialize(mirage::server::ClientAuthorizationRequestEvent&);	
	};
	MIRAGE_CREATE_WITH_EVENT(
		mirage::server::ClientAuthorizationRequestEvent, 
		ClientAuthorization);
}