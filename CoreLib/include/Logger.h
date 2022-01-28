#pragma once

#include <iostream>

class Logger
{
private:
	std::string prefix;

public:
	Logger(const std::string& _prefix)
		: prefix(_prefix)
	{
	}

	void debug(const std::string& str)
	{
		std::cerr << prefix << ": " << str << std::endl;
	}
	
	void error(const std::string& str)
	{
		std::cerr << prefix << ": " << str << std::endl;
	}
};

