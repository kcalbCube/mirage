#pragma once
#include <functional>
#include <algorithm>
#include <boost/signals2.hpp>

namespace mirage
{
	template<typename T, T v>
	struct SignalAny
	{
		using result_type = bool;

		template<typename IT>
		inline bool operator()(IT first, IT last) const
		{
			while(first != last)
				if(*(first++) == v)
					return true;
			return false;
		}
	};

	template<typename T>
	using Signal = boost::signals2::signal<T>;
	template<typename T, typename R>
	using SignalR = boost::signals2::signal<T, R>;
}
