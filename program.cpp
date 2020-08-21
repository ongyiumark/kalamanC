#include <iostream>
#include "Syntax/syntax.h"

int main(int argc, char ** argv)
{
    //double x = 1.1;
    //Objects::Double* val = new Objects::Double(&x);
    //std::cout << val->to_string() << std::endl;
    
    while(true)
    {
        Diagnostics::DiagnosticBag::clear();
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);

        Syntax::Lexer* lexer = new Syntax::Lexer(line);

        Syntax::SyntaxToken* token;
        do
        {
            token = lexer->lex();
            std::cout << Syntax::kind_to_string(token->kind()) << " ";
            if (token->get_object())
            {
                std::cout << token->get_object()->to_string();
            }
            std::cout << std::endl;
        } while (token->kind() != Syntax::SyntaxKind::EndOfFileToken);
        
        Diagnostics::DiagnosticBag::print();
    }
    system("PAUSE");
    return 0;
}