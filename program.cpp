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

        if (show_tree)
        {
            SetConsoleTextAttribute(hConsole, Color::Grey);
            pretty_print(syntax_tree->get_root());
            SetConsoleTextAttribute(hConsole, Color::White);
        }
        
        int n = syntax_tree->get_diagnostics_size();
        if (!n)
        {
            Evaluator e = Evaluator(syntax_tree->get_root());
            
            
            try
            {
                int result = e.evaluate();
                std::cout << result << std::endl;
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
            std::cout << syntax_tree->get_diagnostic(i) << std::endl;
        SetConsoleTextAttribute(hConsole, Color::White);
    }
}