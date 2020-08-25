#include "initialize.h"
#include "../constants.h"
#include <iostream>

using Evaluators::Evaluator;

Contexts::SymbolTable global_symbol_table = Contexts::SymbolTable(nullptr);
Contexts::Context context("<program>", nullptr, global_symbol_table);

// Create Builtin 
void add_builtin_function(std::string name, std::vector<std::string> arg_names)
{
    Objects::Object* func = new Objects::Function(name, arg_names, nullptr, true);
    context.get_symbol_table()->set_object(name, func);
}

void Evaluators::initialize()
{
    add_builtin_function(BI_INPUT, {});
    add_builtin_function(BI_PRINT, {"value"});
    add_builtin_function(BI_TO_INT, {"value"});
}

void Evaluators::run(std::string &script, bool show_tree, bool show_return)
{
    Diagnostics::DiagnosticBag::script = script;
    Syntax::Parser parser(script, show_return);
    Syntax::SyntaxNode* root = parser.parse();

    if (show_tree) Syntax::pretty_print(root);

    Objects::Object* answer = nullptr;
    if (!Diagnostics::DiagnosticBag::size()) 
        answer = Evaluator::evaluate(context, root);

    Diagnostics::DiagnosticBag::print();

    // If the List only has one element, print that element.
    if (!Diagnostics::DiagnosticBag::size() && show_return && answer != nullptr) 
    {
        if (answer->type() == Objects::Type::LIST && ((Objects::List*)answer)->get_size() == 1)
            std::cout << ((Objects::List*)answer)->get_value(0)->to_string();
        else std::cout << answer->to_string();
        std::cout << std::endl;
    }

    delete answer;
    delete root;
    Diagnostics::DiagnosticBag::clear();
    Evaluator::clear();
}