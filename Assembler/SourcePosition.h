
extern int errorCount;

struct SourcePosition
{
	int line;
	std::string file;

	void error(const std::string& s) const
	{
		std::cerr << file << ":" << std::to_string(line) << " :ERROR: " + s << std::endl;
	}
};
