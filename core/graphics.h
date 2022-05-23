#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/serialization/version.hpp>

/*
 * Vertice - graphics element which represent object.
 * Its represent controls by resource id, which maps to Resource.
 */

namespace mirage::graphics
{
	struct Resource
	{
		boost::uuids::uuid uuid;
		std::string path;		

		void serialize(auto& ar, const unsigned version)
		{
			ar & uuid;
			ar & path;
		}
	};
	
	struct Vertice
	{
		float 
			x = 0.f,
			y = 0.f;

		boost::uuids::uuid 
			resourse{}, gid{};

		int8_t  layer = 0;

		void serialize(auto& ar, const unsigned version)
		{
			ar & x;
			ar & y;
			ar & layer;
			ar & resourse;
			ar & gid;	
		}
	};	
}

BOOST_CLASS_VERSION(mirage::graphics::Resource, 1);
BOOST_CLASS_VERSION(mirage::graphics::Vertice, 1);
