#include "CodeAnalysis/code-analysis.h"
#include <iostream>

using namespace CodeAnalysis;

int main(int argc, char **argv)
{
    bool showTree = false;
    
    while(true)
    {
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);

        if (line == "#showtree")
        {
            showTree = !showTree;
            std::cout << (showTree ? "Showing parse tree..." : "Not showing parse tree...") << std::endl;
            continue;
        }

        if (line == "#cls")
        {
            system("CLS");
            continue;
        }

        Lexer lexer = Lexer(line);
        while(true)
        {
            SyntaxToken* token = lexer.next_token();

            if (token->get_kind() == SyntaxKind::EndOfFileToken)
                break;
        
            std::cout << token->get_kind() << " " << token->get_text();
            switch(token->get_kind())
            {
                case SyntaxKind::NumberToken:
                    std::cout << " " << std::any_cast<int>(token->get_value()) << std::endl;
                    break;
                default:
                    std::cout << std::endl;
                    break;
            }
        }
    }
}