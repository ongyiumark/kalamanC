#include <iostream>
#include <fstream>

#include "Evaluators/initialize.h"

using Evaluators::initialize;
using Evaluators::run;

int main(int argc, char ** argv)
{
    initialize();
    if (argc == 1)
    {
        bool show_tree = false;
        bool show_return = true;
        while(true)
        {
            std::string line;
            std::cout << "> ";
            getline(std::cin, line);
            if (line == "#showtree")
            {
                show_tree = !show_tree;
                std::cout << (show_tree ? "Showing parse tree..." : "Not showing parse tree...") << std::endl;
                continue;
            }

            if(line == "#showreturn")
            {
                show_return = !show_return;
                std::cout << (show_return ? "Showing return values..." : "Not showing return values...") << std::endl;
                continue;
            }

            if (line == "#cls")
            {
                system("CLS");
                continue;
            }
            
            run(line, show_tree, show_return);
        }
    }

    std::string filename = argv[1];
	std::ifstream file(filename);
	std::string script, line;
	if (file.is_open())
	{	
		while(getline(file, line))
			script += line + "\n";

		file.close();
	}

    run(script);
    return 0;
}