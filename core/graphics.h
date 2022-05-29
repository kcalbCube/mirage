#pragma once
#include <string>
#include <boost/uuid/uuid.hpp>
#include <boost/serialization/version.hpp>
#include "utility.h"
#include <entt/entt.hpp>
#include <SDL.h>

/*
 * Vertice - graphics element which represent object.
 * Its represent controls by resource id, which maps to Resource.
 */

BOOST_SERIALIZATION_SPLIT_FREE(SDL_Surface*);

namespace boost::serialization
{
	void save(auto& ar, const SDL_Surface*& surface, unsigned int)
	{
		ar & surface->w;
		ar & surface->h;
		ar & surface->pitch;
		ar & serialization::make_array(surface->pixels, surface->h * surface->pitch);
	}
	
	void load(auto& ar, SDL_Surface*& surface, unsigned int)
	{
		decltype(surface->w) w{}, h{};
		ar & w;
		ar & h;

		bool shouldInitialize = surface == nullptr;
		if(shouldInitialize)
			surface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
		else
			SDL_LockSurface(surface);


		ar & surface->pitch;
		ar & serialization::make_array(surface->pixels, surface->h * surface->pitch);

		if(!shouldInitialize)
			SDL_UnlockSurface(surface);
	}
}
namespace mirage::graphics
{
	struct Resource
	{
		entt::id_type id;
		SDL_Surface* surface{nullptr};

		void serialize(auto& ar, const unsigned version)
		{
			ar & id;
			ar & surface;
		}
	};
	
	struct Vertice
	{
		float 
			x = 0.f,
			y = 0.f;

		entt::id_type
			resource;

		boost::uuids::uuid 
			gid{};

		int8_t  layer = 0;

		void serialize(auto& ar, const unsigned version)
		{
			ar & x;
			ar & y;
			ar & layer;
			ar & resource;
			ar & gid;	
		}
	};	
}

BOOST_CLASS_VERSION(mirage::graphics::Resource, 1);
BOOST_CLASS_VERSION(mirage::graphics::Vertice, 1);
