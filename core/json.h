#pragma once

#include <boost/json.hpp>
#include <string>
#include <fstream>

namespace mirage
{
	inline boost::json::value parseJson(const std::string& filename)
	{
		std::ifstream is(filename);
		if(!is)
			return nullptr; // TODO: add error

		boost::json::stream_parser p;
		boost::json::error_code ec;
		std::string line;
		while(std::getline(is, line))
		{
			p.write(line, ec);
			if(ec)
				return nullptr;
		}
		p.finish(ec);
		if(ec)
			return nullptr;
		return p.release();
	}
}
