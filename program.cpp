#include "CodeAnalysis/code-analysis.h"
#include <iostream>
#include <windows.h>
#include <exception>

using namespace CodeAnalysis;

int main(int argc, char **argv)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    enum Color 
    {
        Grey = 8,
        Red = 12,
        White = 15
    };

    bool show_tree = true;
    
    while(true)
    {
        std::string line;
        std::cout << "> ";
        getline(std::cin, line);

        if (line == "#showtree")
        {
            show_tree = !show_tree;
            SetConsoleTextAttribute(hConsole, Color::Grey);
            std::cout << (show_tree ? "Showing parse tree..." : "Not showing parse tree...") << std::endl;
            SetConsoleTextAttribute(hConsole, Color::White);
            continue;
        }

        if (line == "#cls")
        {
            system("CLS");
            continue;
        }

        SyntaxTree* syntax_tree = SyntaxTree::parse(line);
        Binder* binder = new Binder();
        BoundExpression* bound_expression = binder->bind_expression(syntax_tree->get_root());
        std::vector<std::string> diagnostics = syntax_tree->get_diagnostics();
        std::vector<std::string> binder_diagnostics = binder->get_diagnostics();
        diagnostics.insert(diagnostics.end(), binder_diagnostics.begin(), binder_diagnostics.end());

        if (show_tree)
        {
            SetConsoleTextAttribute(hConsole, Color::Grey);
            pretty_print(syntax_tree->get_root());
            SetConsoleTextAttribute(hConsole, Color::White);
        }
        
        int n = diagnostics.size(); 
        if (!n)
        {
            Evaluator e = Evaluator(bound_expression);
            
            try
            {
                std::any any_val = e.evaluate();
                const std::type_info& type = any_val.type();
                if (type == typeid(bool))
                    std::cout << std::any_cast<bool>(any_val);
                else if (type == typeid(int))
                    std::cout << std::any_cast<int>(any_val);
                else 
                    std::cout << "Unknown type " << type.name();
                std::cout << std::endl;
            }
            catch(std::string message)
            {
                SetConsoleTextAttribute(hConsole, Color::Red);
                std::cout << message << std::endl;
                SetConsoleTextAttribute(hConsole, Color::White);
            }
            catch(...)
            {
                SetConsoleTextAttribute(hConsole, Color::Red);
                std::cout << "Unknown error has occured" << std::endl;
                SetConsoleTextAttribute(hConsole, Color::White);
            }
        }

        SetConsoleTextAttribute(hConsole, Color::Red);
        for (int i = 0; i < n; i++)
            std::cout << diagnostics[i] << std::endl;
        SetConsoleTextAttribute(hConsole, Color::White);
    }
}