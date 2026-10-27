#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;
// Calls the function.
FuncCallExpressionSyntax::FuncCallExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxNode*>& args, Position pos)
    : SyntaxNode(pos), _identifier(identifier), _args(args), _callee(nullptr) {}

FuncCallExpressionSyntax::FuncCallExpressionSyntax(SyntaxNode* callee, std::vector<SyntaxNode*>& args, Position pos)
    : SyntaxNode(pos), _identifier(SyntaxToken()), _args(args), _callee(callee) {}

FuncCallExpressionSyntax::~FuncCallExpressionSyntax()
{
    delete _callee;
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

SyntaxNode* FuncCallExpressionSyntax::get_callee()
{
    return _callee;
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