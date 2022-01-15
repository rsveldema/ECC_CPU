#include "Assembler.h"
#include <iostream>
#include <filesystem>
#include "Linker.h"
#include "ObjectWriter.h"

int errorCount;


void Usage()
{
	fprintf(stderr, "asm <infile.asm> <outfile.bin>\n");
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Usage();
		return 1;
	}

	std::string infile = argv[1];
	std::string outfile = argv[2];

	std::cout << "asm: " << std::filesystem::current_path() << std::endl;
	std::cout << "parsing " << infile << " writing output to " << outfile << std::endl;

	Program program;
	program.parse(infile);

	Linker linker;
	linker.link(program.getInsns());

	ObjectWriter::write(program.getInsns(), outfile);

	return 0;
}