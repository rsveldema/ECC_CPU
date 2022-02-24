#pragma once

#include <string>
#include "VectorValue.h"

namespace ecc 
{
    static inline
    std::string to_string(const memory_address_t& v)
    {
        return std::to_string(v);
    }
    
	static std::string to_string(const VectorValue &v)
	{
		std::string s = "<";
		const char *comma = "";
		for (auto &v : v.data)
		{
			s += comma;
			s += std::to_string(v);
			comma = ", ";
		}

		s += ">";
		return s;
	}
	
}
