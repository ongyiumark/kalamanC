#include <iostream>
#include <fstream>
#include "Syntax/Expressions/syntax-expressions.h"
#include "Objects/object.h"
#include "Evaluator/evaluator.h"
#include "Contexts/context.h"
#include "constants.h"

Contexts::SymbolTable* global_symbol_table = new Contexts::SymbolTable(NULL);
Contexts::Context* context = new Contexts::Context("<program>", NULL, global_symbol_table);

void initialize()
{
    std::vector<std::string> arg_names = {};
    context->get_symbol_table()->set_object(BI_INPUT, new Objects::Function(BI_INPUT, arg_names, NULL, true));
    arg_names = {"value"};
    context->get_symbol_table()->set_object(BI_PRINT, new Objects::Function(BI_PRINT, arg_names, NULL, true));
}

int main(int argc, char ** argv)
{
    initialize();
    if (argc == 1)
    {
        while(true)
        {
            std::string line;
            std::cout << "> ";
            getline(std::cin, line);

            Syntax::Parser* parser = new Syntax::Parser(line);
            Syntax::SyntaxNode* root = parser->parse();

            Syntax::pretty_print(root);
            std::cout << Evaluator::evaluate(context, root)->to_string() << std::endl;
            
            
            Diagnostics::DiagnosticBag::print();
            Diagnostics::DiagnosticBag::clear();
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

    Syntax::Parser* parser = new Syntax::Parser(script);
    Syntax::SyntaxNode* root = parser->parse();

    //Syntax::pretty_print(root);

    std::cout << Evaluator::evaluate(context, root)->to_string() << std::endl;
    
    Diagnostics::DiagnosticBag::print();
    return 0;
}