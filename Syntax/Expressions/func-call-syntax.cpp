#include "syntax-expressions.h"

using namespace Syntax;

// Calls the function.
FuncCallExpressionSyntax::FuncCallExpressionSyntax(SyntaxToken* identifier, std::vector<SyntaxNode*>& args)
    : _identifier(identifier), _args(args) {}

// Tokens are already deleted by the parser.
FuncCallExpressionSyntax::~FuncCallExpressionSyntax()
{
    for (auto &o : _args)
        delete o;
}

SyntaxKind FuncCallExpressionSyntax::kind() const
{
    return SyntaxKind::FuncCallExpression;
}

SyntaxToken* FuncCallExpressionSyntax::get_identifier() const
{
    return _identifier;
}

int FuncCallExpressionSyntax::get_arg_size() const
{
    return _args.size();
}

SyntaxNode* FuncCallExpressionSyntax::get_arg(int i) const
{
    return _args[i];
}

std::vector<SyntaxNode*> FuncCallExpressionSyntax::get_args() const
{
    return _args;
}