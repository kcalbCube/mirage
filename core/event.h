#pragma once
#include <entt/entt.hpp>
#include "static.h"
#include "utility.h"

/*
 * on event _event_ create component _T_ with event as argument
 */

#define MIRAGE_CREATE_WITH_EVENT(_event_, _T_) \
	namespace __static__##_T_##_event_{ \
	MIRAGE_ON_STARTUP(createWithEvent, ( \
		::mirage::event::emitter().on<_event_>([](_event_& event, \
				const ::mirage::event::Emitter&) -> void { \
			(void)::mirage::ecs::create<_T_>(event); \
		})))};
/*
 * on event _event_ create component _T_.
 */
#define MIRAGE_CREATE_ON_EVENT(_event_, _T_) \
	namespace __static__##_T_##_event_{ \
	MIRAGE_ON_STARTUP(createOnEvent, ( \
		::mirage::event::emitter().on<_event_>([](_event_& event, \
				const ::mirage::event::Emitter&) -> void { \
			(void)::mirage::ecs::create<_T_>(); \
		})))};

namespace mirage::event
{
	MIRAGE_COFU(entt::dispatcher, dispatcher);

	class Emitter : public entt::emitter<Emitter>
	{
	};

	MIRAGE_COFU(Emitter, emitter);

	struct IEventConnectionGuard
	{
		virtual ~IEventConnectionGuard(void) = default;
	};
	template<typename Event>
	class EventConnectionGuard : public IEventConnectionGuard
	{
		Emitter::connection<Event> connection;
	public:
		EventConnectionGuard(Emitter::connection<Event> con)
			: connection { std::move(con) } {}
		virtual ~EventConnectionGuard(void) override
		{
			emitter().erase(connection);
		}
	};
}
