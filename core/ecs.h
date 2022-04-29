#pragma once
#include <cstdint>
#include <memory>
#include <mutex>
#include "event.h"
#include <concepts>

namespace mirage::ecs
{
	MIRAGE_COFU(entt::registry, registry);
	template<typename T>
	class Component
	{
		static std::once_flag onceFlag;
		std::vector<std::shared_ptr<event::IEventConnectionGuard>> 
			connectionGuards;	
	protected:
		Component(void) = default;
		Component(auto...) = delete;
	public:
		entt::entity entity;
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
		void initialize(entt::entity, Args&&...) {}

		void onDestroy(void) {}

		void destroy(void);

		template<typename Event>
		void bindEvent(auto function)
		{
			connectionGuards.push_back(
				std::make_shared<event::EventConnectionGuard<Event>> (
					event::emitter().on<Event>(boost::bind(
						boost::mem_fn(function), static_cast<T*>(this), _1)
					)
				)
			);
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
			: entity{entity_} {}
		ComponentWrapper(void) = default;
		ComponentWrapper(ComponentWrapper<T>&&) = delete;
		ComponentWrapper(const ComponentWrapper<T>&) = delete;
		ComponentWrapper& operator=(const entt::entity& other);
		T* tryGet(void);
		const T* tryGet(void) const;	
		bool isValid(void) const;	
		operator bool(void) const;	
		operator T&(void);	
		operator const T&(void) const;	
		T& get(void);
		const T& get(void) const;	
		template<typename Y>
			requires std::derived_from<Y, Component<T>>
		ComponentWrapper<T>& operator=(const Y& other)
		{
			entity = entt::to_entity(registry(), other);
		}
	};	
}
template<typename T>
inline mirage::ecs::ComponentWrapper<T>& 
       mirage::ecs::ComponentWrapper<T>::operator=(const entt::entity& other)
{
	return entity = other;
}
template<typename T>
inline T* mirage::ecs::ComponentWrapper<T>::tryGet(void)
{
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
}

template<typename T>
inline void mirage::ecs::Component<T>::destroy(void)
{
	mirage::ecs::destroy<T>(entt::to_entity(registry(), *this));
}
