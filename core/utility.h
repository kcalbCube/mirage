#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <algorithm>
#include <string>
#include <boost/range/algorithm_ext/erase.hpp>

#ifndef _DEBUG
#if defined(_MSC_VER)
#define UNREACHABLE __assume(0)
#elif defined(__GNUC__)
#define UNREACHABLE __builtin_unreachable()
#else
#define UNREACHABLE abort()
#endif
#else
#define UNREACHABLE abort()
#endif

using namespace boost::placeholders;
namespace mirage::utils
{
	inline std::string sanitize(std::string str, std::function<bool(char)> filter)
	{
		return boost::remove_erase_if(str, !boost::bind(filter, _1));
	}

	inline auto sanitizeAlnum = boost::bind(sanitize, _1, isalnum);
	inline auto sanitizeUsername = sanitizeAlnum;
}
