#pragma once

#include <iostream>

namespace ecc
{
	extern std::string __logging_prefix;

	static inline void $display(const std::string &str)
	{
		std::cerr << __logging_prefix << ": " << str << std::endl;
	}

	static inline void $error(const std::string &str)
	{
		std::cerr << __logging_prefix << ": " << str << std::endl;
	}

}