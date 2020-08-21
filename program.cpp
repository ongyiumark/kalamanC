#include <iostream>
#include "Syntax/Expressions/syntax-expressions.h"


int main(int argc, char ** argv)
{
    while(true)
    {
        Diagnostics::DiagnosticBag::clear();
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);

        Syntax::Parser* parser = new Syntax::Parser(line);
        Syntax::SyntaxNode* root = parser->parse();

        Syntax::pretty_print(root);
        Diagnostics::DiagnosticBag::print();
    }
    system("PAUSE");
    return 0;
}