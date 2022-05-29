#pragma once
#include "core/processing.h"
#include "core/static.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <core/ecs.h>
#include <core/event.h>

struct MockComponent : public mirage::ecs::Component<MockComponent>	
{
	int 
		initializeNumber = 0,
		lateInitializeNumber = 0,
		f1Number = 0;
	static int 
		staticInitializeNumber;

	static void staticInitialize(void)
	{
		++staticInitializeNumber;
	}

	void initialize(void)
	{
		++initializeNumber;
	}

	void lateInitialize(void)
	{
		++lateInitializeNumber;
	}

	void f1(int&)
	{
		++f1Number;
	}	
};
inline int MockComponent::staticInitializeNumber = 0;

TEST(Ecs, Initialize)
{
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();
	
	EXPECT_EQ(created.initializeNumber, 1);
	
	created.destroy();
}

TEST(Ecs, StaticInitialize)
{
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();
	
	EXPECT_EQ(created.staticInitializeNumber, 1);
	
	created.destroy();
}

TEST(Ecs, LateInitializeManually)
{	
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();

	mirage::ecs::lateQueueUpdate();

	EXPECT_GE(created.lateInitializeNumber, 1);

	created.destroy();
}

TEST(Ecs, LateInitializeByDispatcher)
{	
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();

	std::this_thread::sleep_for(std::chrono::milliseconds(
		mirage::ecs::processing::EventDispatcherProcessing::updatePeriod * 2));

	EXPECT_GE(created.lateInitializeNumber, 1);

	created.destroy();
}

TEST(Ecs, EventBindTrigger)
{
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();

	created.bindEvent<int, &MockComponent::f1>();

	mirage::event::triggerEvent<int>(1);

	EXPECT_EQ(created.f1Number, 1);

	created.destroy();
}

TEST(Ecs, EventBindUnbindTrigger)
{
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();

	created.bindEvent<int, &MockComponent::f1>();

	mirage::event::triggerEvent<int>(1);
	
	EXPECT_EQ(created.f1Number, 1);

	created.unbindEvent<int>();
	
	mirage::event::triggerEvent<int>(1);
	
	EXPECT_EQ(created.f1Number, 1);

	created.destroy();
}

TEST(Ecs, EventBindEnqueue)
{
	auto& created = mirage::ecs::ComponentWrapper<MockComponent>::create().get();

	created.bindEvent<int, &MockComponent::f1>();

	mirage::event::enqueueEvent<int>(1);
	std::this_thread::sleep_for(std::chrono::milliseconds(
		mirage::ecs::processing::EventDispatcherProcessing::updatePeriod * 2));

	EXPECT_EQ(created.f1Number, 1);

	created.destroy();
}

template<int I> struct Tag {};

template<int I>
struct StaticMock : mirage::ecs::Component<StaticMock<I>>
{
	static bool initializeds;
	void initialize(void)
	{
		initializeds = true;
	}
};

template<>
struct StaticMock<2> : mirage::ecs::Component<StaticMock<2>>
{
	static bool initializeds;
	void initialize(Tag<2>&)
	{
		initializeds = true;
	}
};

template<int I>
inline bool StaticMock<I>::initializeds = false;
inline bool StaticMock<2>::initializeds = false;

using Tag1 = Tag<1>;
using StaticMock1 = StaticMock<1>;
using Tag2 = Tag<2>;
using StaticMock2 = StaticMock<2>;

MIRAGE_CREATE_ON_STARTUP(StaticMock<0>, staticOnStartMock);
MIRAGE_CREATE_ON_EVENT(Tag1, StaticMock1);
MIRAGE_CREATE_WITH_EVENT(Tag2, StaticMock2);

TEST(Ecs, StaticOnStart)
{
	EXPECT_EQ(StaticMock<0>::initializeds, true);
}

TEST(Ecs, StaticOnEvent)
{
	EXPECT_NE(StaticMock1::initializeds, true);
	mirage::event::enqueueEvent<Tag1>();
	std::this_thread::sleep_for(std::chrono::milliseconds(
		mirage::ecs::processing::EventDispatcherProcessing::updatePeriod * 2));
	EXPECT_EQ(StaticMock1::initializeds, true);
}

TEST(Ecs, StaticWithEvent)
{
	EXPECT_NE(StaticMock2::initializeds, true);
	mirage::event::enqueueEvent<Tag2>();
	std::this_thread::sleep_for(std::chrono::milliseconds(
		mirage::ecs::processing::EventDispatcherProcessing::updatePeriod * 2));
	EXPECT_EQ(StaticMock2::initializeds, true);
}

