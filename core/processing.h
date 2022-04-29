#pragma once
#include <mutex>
#include <thread>
#include "utility.h"
#include <chrono>
#include <entt/entt.hpp>
#include <entt/process/scheduler.hpp>
#include "ecs.h"

namespace mirage::ecs::processing
{
	template<typename DeltaType_>
	class Processor
	{
		std::once_flag onceFlag;	
	public:	
		using DeltaType = DeltaType_;	
		entt::scheduler<DeltaType_> scheduler;

		void start(void) {};
		void stop(void) {};

		void requestStop(void);
		virtual ~Processor(void);
	};

	template<unsigned Milliseconds>
	class PeriodMS : public Processor<unsigned>
	{
		PeriodMS(void) = default;
		std::jthread thread;
	public:

		void start(void);	

		void stop(void) { thread.request_stop(); }

		static PeriodMS<Milliseconds>& getInstance(void)
		{
			static PeriodMS<Milliseconds> instance;
			return instance;
		}
	};

	template<unsigned Delim=100>
	void doAfter(unsigned milliseconds, std::function<void(void)> function);	
}

namespace mirage::ecs
{
	template<typename T>
	struct Processing
	{
		template<typename Derived, typename DeltaType>
		class Process : public entt::process<Process<Derived, DeltaType>, DeltaType>
		{
		protected:
			ComponentWrapper<T> parent;
		public:
			Process(entt::entity parent_)
				: parent{std::move(parent_)} {}
		};

		template<typename ProcessT, typename DeltaType, typename... Args>
			requires std::derived_from<ProcessT, Process<ProcessT, DeltaType>>
		void startProcess(processing::Processor<DeltaType>& processor, Args&&... args)
		{
			processor.scheduler.template attach<ProcessT>(entt::to_entity(registry(), *this));
		}

		template<typename ProcessT, typename DeltaType, typename... Args>
		void startProcess(processing::Processor<DeltaType>& processor, Args&&... args)
		{
			processor.scheduler.template attach<ProcessT>(args...);
		}
	};
}

template<unsigned Delim>
inline void mirage::ecs::processing::doAfter(
	unsigned milliseconds, 
	std::function<void(void)> function)
{
	if(milliseconds < Delim)
	{
		if(Delim <= 1)
		{
			abort();
		}

		doAfter<Delim / 10>(milliseconds, function);
		return;
	}

	class Process : public entt::process<Process, unsigned>
	{
		std::function<void(void)> function;
		unsigned forSucceed = 0, counter = 0;
	public:
		Process(std::function<void(void)> function_, unsigned forSucceed_)
			: function { std::move(function_) },
			forSucceed { forSucceed_ } {}

		void update(unsigned delta, void*)
		{
			if(counter++ > forSucceed)
				entt::process<Process, unsigned>::succeed();
		}

		void succeeded(void)
		{
			function();
		}
	};

	PeriodMS<Delim>::getInstance().scheduler
		.template attach<Process>(function, milliseconds / Delim);
}

template<typename DeltaType_>
inline void mirage::ecs::processing::Processor<DeltaType_>::requestStop(void)
{
	std::call_once(onceFlag, 
		boost::bind(&Processor::stop, this));
}
template<typename DeltaType_>
inline mirage::ecs::processing::Processor<DeltaType_>::~Processor(void) 
{
	requestStop();
	scheduler.clear();
}

template<unsigned Milliseconds>
inline void mirage::ecs::processing::PeriodMS<Milliseconds>::start(void)
{
	thread = std::jthread([*this](std::stop_token itoken) mutable -> void
	{
		while(!itoken.stop_requested())
		{
			auto start = std::chrono::high_resolution_clock::now();
			scheduler.update(Milliseconds);
			std::this_thread::sleep_until(start +
				std::chrono::milliseconds(Milliseconds));
		};
	});

	thread.detach();
}
