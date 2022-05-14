#pragma once
#define BOOST_BIND_GLOBAL_PLACEHOLDERS
#include <boost/bind.hpp>
#include <algorithm>
#include <string>
#include <boost/range/algorithm_ext/erase.hpp>
#include "static.h"
#include <magic_enum.hpp>
#include "logging.h"
#define MIRAGE_COFU(T, name, ...) \
	inline T & name (void) { static T instance{ __VA_ARGS__ }; return instance; }
#define MIRAGE_COFU_CONST(T, name, ...) \
	inline const T & name (void) { static T instance{ __VA_ARGS__ }; return instance; }
	
using namespace boost::placeholders;
namespace mirage::utils
{
	inline std::string sanitize(std::string str, std::function<bool(char)> filter)
	{
		return boost::remove_erase_if(str, !boost::bind(filter, _1));
	}
	
	inline std::string sanitizeUsername(std::string_view str)
	{
		return sanitize(std::string{str}, isalnum);
	}
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

	/*
	 * safe stringView to memarray, 
	 * returns "" if string invalid, if valid returned 
	 * string_view size will be equal to strlen(src)
	 */
	inline std::string_view stringView(const char* const src, const size_t maxSize)
	{
		if(!src)
			return "";
		const auto* const zeroPos = 
			static_cast<const char* const>(memchr(src, '\0', maxSize));
		if(!zeroPos)
			return "";

		const size_t size = static_cast<size_t>(zeroPos - src);

		if(!size)
			return "";

		return std::string_view(src, size);
	}
}
