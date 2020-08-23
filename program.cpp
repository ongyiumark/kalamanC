#include <iostream>
#include <fstream>

#include "Evaluator/evaluator.h"
#include "constants.h"

Contexts::SymbolTable* global_symbol_table = new Contexts::SymbolTable(NULL);
Contexts::Context* context = new Contexts::Context("<program>", NULL, global_symbol_table);

// Create Builtin 
void initialize()
{
    std::vector<std::string> arg_names = {};
    context->get_symbol_table()->set_object(BI_INPUT, new Objects::Function(BI_INPUT, arg_names, NULL, true));
    arg_names = {"value"};
    context->get_symbol_table()->set_object(BI_PRINT, new Objects::Function(BI_PRINT, arg_names, NULL, true));
    context->get_symbol_table()->set_object(BI_TO_INT, new Objects::Function(BI_TO_INT, arg_names, NULL, true));
}

void run(std::string &script, bool show_tree=false, bool show_return=false)
{
    Syntax::Parser* parser = new Syntax::Parser(script, show_return);
    Syntax::SyntaxNode* root = parser->parse();
    
    if (show_tree) Syntax::pretty_print(root);

    Objects::Object* answer = Objects::Object::none_result;
    if (!Diagnostics::DiagnosticBag::should_return()) 
        answer = Evaluator::evaluate(context, root);

    Diagnostics::DiagnosticBag::print();

    // If the List only has one element, print that element.
    if (!Diagnostics::DiagnosticBag::should_return() && show_return) 
    {
        if (answer->type() == Objects::Type::LIST && ((Objects::List*)answer)->get_size() == 1)
            std::cout << ((Objects::List*)answer)->get_value(0)->to_string();
        else std::cout << answer->to_string();
        std::cout << std::endl;
    }

    // Plugging that memory leak.
    delete parser;
    delete root;
    if (answer != Objects::Object::none_result) delete answer;
}

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

    run(script);

    return 0;
}