#include "CodeAnalysis/code-analysis.h"
#include <iostream>

using namespace CodeAnalysis;

int main(int argc, char **argv)
{
    bool show_tree = true;
    
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

        if (line == "#cls")
        {
            system("CLS");
            continue;
        }

        SyntaxTree* syntax_tree = SyntaxTree::parse(line);

        if (show_tree)
            pretty_print(syntax_tree->get_root());
        
        int n = syntax_tree->get_diagnostics_size();
        if (!n)
        {
            Evaluator e = Evaluator(syntax_tree->get_root());
            int result = e.evaluate();
            std::cout << result << std::endl;
        }

        for (int i = 0; i < n; i++)
            std::cout << syntax_tree->get_diagnostic(i) << std::endl;
    }
}