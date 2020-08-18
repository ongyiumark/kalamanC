#include "init.h"
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

// Runs the kalamansi script
RTResult run(std::string filename, std::string script, bool show_output)
{
	RTResult result = RTResult();

	LexerResult lex_result = Lexer(filename, script).make_tokens();
	if (lex_result.get_error()) 
	{
		result.failure(lex_result.get_error());
		return result;
	}

	//std::cout << lex_result << std::endl;

	ParserResult parse_result = Parser(lex_result.get_tokens(), show_output).parse();
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
	RTResult result = run(filename, script, false);
	if (result.get_error())
		std::cout << result << std::endl;
}

void set_builtins()
{
	// Variables
	global_context->get_table()->set_value(BUILTINNAMES[BuiltInName::V_TRUE], new Integer(1));
	global_context->get_table()->set_value(BUILTINNAMES[BuiltInName::V_FALSE], new Integer(0));

	// Functions
	std::vector<std::string> args = {"value"};
	global_context->get_table()->set_value(BUILTINNAMES[BuiltInName::PRINT], 
		new BuiltInFunction(BuiltInName::PRINT, args));

	args = {};
	global_context->get_table()->set_value(BUILTINNAMES[BuiltInName::INPUTSTR], 
		new BuiltInFunction(BuiltInName::INPUTSTR, args));
}