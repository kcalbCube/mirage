#include <gtest/gtest.h>
#include "serialization.h"
#include "ecs.h"

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}
