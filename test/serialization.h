#pragma once
#include <gtest/gtest.h>
#include <core/utility.h>

TEST(Serialization, SerializeAndDeserialize)
{	
	int object{1};

	auto serialized = mirage::utils::serialize(object);
	auto deserialized = mirage::utils::deserialize<int>(serialized);

	EXPECT_EQ(object, deserialized);
}
