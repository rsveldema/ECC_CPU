#pragma once

#include <iostream>
#include <string>

#include "basic_display.h"

namespace ecc
{
	template<typename T, typename T2>
	static inline void $display(const std::string &str, const T arg, const T2 arg2)
	{
		std::cerr << __logging_prefix << ": " << str <<  "\t" <<  ecc::to_string(arg) << "\t" <<  ecc::to_string(arg2) << std::endl;
	}

	template<typename T, typename T2, typename T3>
	static inline void $display(const std::string &str, const T arg, const T2 arg2, const T3 arg3)
	{
		std::cerr << __logging_prefix << ": " << str <<  "\t" <<  ecc::to_string(arg) << "\t" <<  ecc::to_string(arg2)<< "\t" <<  ecc::to_string(arg3) << std::endl;
	}

	template<typename T, typename T2, typename T3, typename T4>
	static inline void $display(const std::string &str, const T arg, const T2 arg2, const T3 arg3, const T4 arg4)
	{
		std::cerr << __logging_prefix << ": " << str <<  "\t" <<  ecc::to_string(arg) << "\t" <<  ecc::to_string(arg2) << "\t" <<  ecc::to_string(arg3) << "\t" <<  ecc::to_string(arg4) << std::endl;
	}

	static inline void $error(const std::string &str)
	{
		std::cerr << __logging_prefix << ": " << str << std::endl;
	}

}