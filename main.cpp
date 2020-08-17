#include "lexer/lexer.h"
#include "parser/parser.h"
#include "interpreter/interpreter.h"

#include "tracers/context.h"
#include "tracers/symbol_table.h"
#include "constants.h"

#include <iostream>
#include <fstream>

// Sets the global context
SymbolTable* global_symbol_table = new SymbolTable();
Context* global_context = new Context("<program>", global_symbol_table);

// Runs the kalman script
RTResult run(std::string filename, std::string script)
{
	RTResult result = RTResult();

	LexerResult lex_result = Lexer(filename, script).make_tokens();
	if (lex_result.get_error()) 
	{
		result.failure(lex_result.get_error());
		return result;
	}
	//std::cout << lex_result << std::endl;
	//std::cout << script << std::endl;

	ParserResult parse_result = Parser(lex_result.get_tokens()).parse();
	if (parse_result.get_error()) 
	{
		result.failure(parse_result.get_error());
		return result;
	}
	//std::cout << parse_result << std::endl;

	result = parse_result.get_node()->visit(global_context);
	return result;
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
	RTResult result = run("<stdin>", script);
	if (result.get_error())
		std::cout << result << std::endl;
}

void set_builtins()
{
	std::vector<std::string> args = {"value"};
	global_context->get_table()->set_value(BUILTINNAMES[BuiltInName::PRINT], 
		new BuiltInFunction(BuiltInName::PRINT, args));

	args = {};
	global_context->get_table()->set_value(BUILTINNAMES[BuiltInName::INPUTSTR], 
		new BuiltInFunction(BuiltInName::INPUTSTR, args));
}

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
			RTResult result = run("<stdin>", script);
			std::cout << result << std::endl;
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