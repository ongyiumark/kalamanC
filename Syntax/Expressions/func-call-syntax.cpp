#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;
// Calls the function.
FuncCallExpressionSyntax::FuncCallExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxNode*>& args, Position pos)
    : SyntaxNode(pos), _identifier(identifier), _args(args) {}

FuncCallExpressionSyntax::~FuncCallExpressionSyntax()
{
    for (auto &o : _args)
        delete o;
}

SyntaxKind FuncCallExpressionSyntax::kind() const
{
    return SyntaxKind::FuncCallExpression;
}

SyntaxToken* FuncCallExpressionSyntax::get_identifier()
{
    return &_identifier;
}

int FuncCallExpressionSyntax::get_arg_size()
{
    return _args.size();
}

SyntaxNode* FuncCallExpressionSyntax::get_arg(int i)
{
    return _args[i];
}

std::vector<SyntaxNode*> FuncCallExpressionSyntax::get_args()
{
    return _args;
}