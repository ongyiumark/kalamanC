#include <iostream>
#include <fstream>

#include "lexer.h"

// Runs the kalman script
void run(std::string filename, std::string script)
{
	LexerResult lex_result = Lexer(filename, script).make_tokens();
	// std::cout << lex_result << std::endl;
}

// Reads file and runs
void run_file(std::string filename)
{
	std::ifstream file(filename);
	std::string script, line;
	if (file.is_open())
	{	
		while(getline(file, line))
			script += line + "\n";

		file.close();
	}
	run(filename, script);
}

int main(int argc, char const *argv[])
{
	// Read script from console input
	if (argc == 1)
	{
		while(1)
		{
			printf("kalamansi >> ");
			std::string script;
			getline(std::cin, script);
			run("<stdin>", script);
		}
		return 0;
	}

	if (argc != 2)
	{
		exit(1);	
	}

	// Reads kalman script from file
	run_file(argv[1]);
	
	return 0;
}