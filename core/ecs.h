#pragma once
#include <cstdint>
#include <memory>
#include "event.h"
#include <concepts>
#include <deque>
#include <boost/function.hpp>

namespace mirage::ecs
{
	MIRAGE_COFU(entt::registry, registry);
	MIRAGE_COFU(std::mutex, lateQueueLock);
	MIRAGE_COFU(std::deque<boost::function<void(void)>>, lateQueue);

	template<typename T>
	class Component
	{
		static std::once_flag onceFlag;
		
	protected:
		Component(void) = default;
		Component(auto...) = delete;
		void initialize(void);
	public:
		bool initialized = false;

		entt::entity entity{};

		virtual ~Component(void);
		template<typename CT, typename... Args>
			requires std::derived_from<CT, Component<CT>>
		friend void initialize(entt::entity, Args&&...);	

		/*
		 * called once before first creation
 	 	 */
		static void staticInitialize(void) {}

		/*
		 * called per creation
		 */
		template<typename... Args>
		void initialize(Args&&...) {}

		/*
		 * use this for event bindings, event enquements, etc
		 *
		 * expl: this is a stub, used to determine existing of late initializator by invoke_result,
		 * 	override this with void return value
		 */
		std::nullptr_t lateInitialize(void)
		{ 
			return nullptr; 
		}

		void onDestroy(void) {}

		void destroy(void);

		void callLate(void (T::*func)(void))
		{
			std::lock_guard lock(lateQueueLock());
			lateQueue().push_back(boost::bind(func, static_cast<T*>(this)));
		}

		void callLate(boost::function<void(void)> function)
		{
			std::lock_guard lock(lateQueueLock());
			lateQueue().push_back(std::move(function));
		}

		/*
		 * cannot be called in event handler 
		 */
		template<typename Event, void (T::*Func)(Event&)>
		void bindEvent(void)
		{	
			event::dispatcher().sink<Event>()
				.template connect<Func>(static_cast<T*>(this));	
		}

		template<typename Event, void (T::*Func)(Event&)>
		void bindEventLate(void)
		{
			callLate(boost::bind(event::dispatcher().sink<Event>().template connect<Func>,
				&event::dispatcher().sink<Event>(), static_cast<T*>(this)));
		}	

		template<typename Event, void (T::*Func)(const Event&)>
		void bindEvent(void);
		template<typename Event, void (T::*Func)(Event)>
		void bindEvent(void);

		template<typename Event>
		void unbindEvent(void)
		{	
			event::dispatcher().disconnect<Event>(static_cast<T*>(this));
		}

		template<typename Event>
		void unbindEventLate(void)
		{
			callLate(boost::bind(event::dispatcher().template disconnect<Event>,
				&event::dispatcher(), static_cast<T*>(this)));
		}
	};	

	template<typename T>	
	entt::entity createUnitialized(void);

	/*
	 * initialize component, created by createUnitialized()
	 */
	template<typename T, typename... Args>
		requires std::derived_from<T, Component<T>>
	void initialize(entt::entity, Args&&...);

	/*
	 * create entity with T component
	 */
	template<typename T, typename... Args>
		requires std::derived_from<T, Component<T>>
	entt::entity create(Args&&... args);	

	/*
	 * create entity with T component
	 */
	template<typename T, typename... Args>
	entt::entity create(Args&&... args);

	/*
	 * destroys entity
	 */
	template<typename T>
	void destroy(const entt::entity& entity);

	/*
	 * fluffy wrapper for entity component
	 */
	template<typename T>
	struct ComponentWrapper
	{
		entt::entity entity = entt::null;
	public:
		ComponentWrapper(entt::entity entity_)
			: entity{std::move(entity_)} {}
		ComponentWrapper(void) = default;
		ComponentWrapper(const ComponentWrapper<T>&) = default;
		ComponentWrapper(ComponentWrapper&&) = default;
		ComponentWrapper<T>& operator=(ComponentWrapper<T>&&) = default;
		ComponentWrapper<T>& operator=(const entt::entity& other);
		
		T*       tryGet  (void);
		const T* tryGet  (void) const;
		bool     isValid (void) const;

		operator bool    (void) const;

		operator T&      (void);
		operator const T&(void) const;

		T&       get     (void);
		const T& get     (void) const;

		template<typename Y>
			requires std::derived_from<Y, Component<T>>
		ComponentWrapper<T>& operator=(const Y& other)
		{
			entity = entt::to_entity(registry(), other);
		}

		T* operator->(void) { return tryGet(); }
	};		

	template<typename T, typename Y>
	void _createStubArg(Y arg) { create<T>(arg); }
	template<typename T>
	void _createStub(void) { create<T>(); }
}
template<typename T>
inline mirage::ecs::ComponentWrapper<T>& 
       mirage::ecs::ComponentWrapper<T>::operator=(const entt::entity& other)
{
	entity = other;

	return *this;
}
template<typename T>
inline T* mirage::ecs::ComponentWrapper<T>::tryGet(void)
{
	if(!isValid())
		return nullptr;
	return registry().try_get<T>(entity);
}
template<typename T>
inline const T* mirage::ecs::ComponentWrapper<T>::tryGet(void) const
{
	return tryGet();
}
template<typename T>
inline T& mirage::ecs::ComponentWrapper<T>::get(void)
{
	return registry().get<T>(entity);
}
template<typename T>
inline const T& mirage::ecs::ComponentWrapper<T>::get(void) const
{
	return get();
}
template<typename T>
inline mirage::ecs::ComponentWrapper<T>::operator const T&(void) const
{
	return get();
}
template<typename T>
inline mirage::ecs::ComponentWrapper<T>::operator T&(void)
{
	return get();
}
template<typename T>
inline mirage::ecs::ComponentWrapper<T>::operator bool(void) const
{
	return isValid();
}
template<typename T>
inline bool mirage::ecs::ComponentWrapper<T>::isValid(void) const
{
	return registry().valid(entity);
}
template<typename T>
inline std::once_flag mirage::ecs::Component<T>::onceFlag;

template<typename T>
inline entt::entity mirage::ecs::createUnitialized(void)
{
	auto&& entity = registry().create();
	registry().emplace<T>(entity);

	return entity;
}

template<typename T, typename... Args>
	requires std::derived_from<T, mirage::ecs::Component<T>>
inline void mirage::ecs::initialize(entt::entity entity, Args&&... args)
{
	if(auto* entityptr = registry().try_get<T>(entity))
	{
		std::call_once(T::onceFlag, T::staticInitialize);
		entityptr->entity = entity;
		entityptr->initialize(args...);

		if constexpr(std::is_void_v<decltype(entityptr->lateInitialize())>) // late initialize overrided
			entityptr->callLate(&T::lateInitialize);	
	}
}

template<typename T, typename... Args>
	requires std::derived_from<T, mirage::ecs::Component<T>>
inline entt::entity mirage::ecs::create(Args&&... args)
{
	auto&& entity = createUnitialized<T>();
	initialize<T>(entity, args...);	

	return entity;
}

template<typename T, typename... Args>
inline entt::entity mirage::ecs::create(Args&&... args)
{
	return createUnitialized<T>(args...);	
};

template<typename T>
inline void mirage::ecs::destroy(const entt::entity& entity)
{
	registry().destroy(entity);
}

template<typename T>
inline mirage::ecs::Component<T>::~Component(void)
{
	onDestroy();
	event::dispatcher().disconnect(this);
}

template<typename T>
inline void mirage::ecs::Component<T>::destroy(void)
{
	if(!registry().valid(entity))
		return;
	mirage::ecs::destroy<T>(entity);
}
