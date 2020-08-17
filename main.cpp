#include "init.h"

int main(int argc, char const *argv[])
{
	set_builtins();

	// Read script from console input
	if (argc == 1)
	{
		while(1)
		{
			printf("kalamansi >> ");
			std::string script;
			getline(std::cin, script);
			script += ";";		// You'll forget, trust me
			RTResult result = run("<stdin>", script, true);
			std::cout << result << std::endl;
		}
		return 0;
	}

	// Reads kalman script from file
	run_file(argv[1]);
	
	return 0;
}