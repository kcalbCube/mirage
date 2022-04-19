#pragma once
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <cstddef>
namespace mirage
{
	constexpr int version = 1;

	inline boost::uuids::random_generator uuidGenerator;
}
