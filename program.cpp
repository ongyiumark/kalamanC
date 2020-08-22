#include <iostream>
#include <fstream>
#include "Syntax/Expressions/syntax-expressions.h"
#include "Objects/object.h"
#include "Evaluator/evaluator.h"
#include "Contexts/context.h"

int main(int argc, char ** argv)
{
    Contexts::SymbolTable* global_symbol_table = new Contexts::SymbolTable(NULL);
    Contexts::Context* context = new Contexts::Context("<program>", NULL, -1, global_symbol_table);
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