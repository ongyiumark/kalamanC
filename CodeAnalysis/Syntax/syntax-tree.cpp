#include "syntax.h"

using namespace CodeAnalysis;

SyntaxTree::SyntaxTree(const std::vector<std::string>& diagnostics, ExpressionSyntax* root, SyntaxToken* endoffile_token) 
    : _diagnostics(diagnostics), _root(root), _endoffile_token(endoffile_token) {}

ExpressionSyntax* SyntaxTree::get_root()
{
    return _root;
}

SyntaxToken* SyntaxTree::get_endoffile_token()
{
    return _endoffile_token;
}

int SyntaxTree::get_diagnostics_size() const
{
    return _diagnostics.size();
}

std::string SyntaxTree::get_diagnostic(int i) const
{
    int n = _diagnostics.size();
    if (n == 0) return NULL;
    if (i < 0) return _diagnostics[0];
    if (i >= n) return _diagnostics[n-1];
    return _diagnostics[i];
}

std::vector<std::string> SyntaxTree::get_diagnostics() const
{
    return _diagnostics;
}

SyntaxTree* SyntaxTree::parse(const std::string& text)
{
    Parser parser = Parser(text);
    return parser.parse();
}