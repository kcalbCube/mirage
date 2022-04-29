#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <algorithm>
#include <string>
#include <boost/range/algorithm_ext/erase.hpp>
#include "static.h"
#include <magic_enum.hpp>
#include "logging.h"
#define MIRAGE_COFU(T, name) inline T & name (void) { static T instance; return instance; }

using namespace boost::placeholders;
namespace mirage::utils
{
	inline std::string sanitize(std::string str, std::function<bool(char)> filter)
	{
		return boost::remove_erase_if(str, !boost::bind(filter, _1));
	}

	inline auto sanitizeAlnum = boost::bind(sanitize, _1, isalnum);
	inline auto sanitizeUsername = sanitizeAlnum;
/*
 *
 * @example: template<> struct fmt::formatter<MyEnum> : EnumFormatter<MyEnum> {};
 */
	template<typename T>
	struct EnumFormatter : fmt::formatter<std::string_view>
	{
		template<typename FormatContext>
		auto format(T en, FormatContext& ctx)
		{
			return fmt::formatter<std::string_view>::format(magic_enum::enum_name(en), ctx);
		}
	};
}
