#include "syntax-expressions.h"

using namespace Syntax;

CallExpressionSyntax::CallExpressionSyntax(SyntaxToken* identifier, std::vector<SyntaxNode*>& args)
    : _identifier(identifier), _args(args) 
{
    _children = {identifier};
    _children.insert(_children.end(), args.begin(), args.end());
}

SyntaxKind CallExpressionSyntax::kind() const
{
    return SyntaxKind::CallExpression;
}

SyntaxToken* CallExpressionSyntax::get_identifier() const
{
    return _identifier;
}

int CallExpressionSyntax::get_arg_size() const
{
    return _args.size();
}

SyntaxNode* CallExpressionSyntax::get_arg(int i) const
{
    return _args[i];
}

std::vector<SyntaxNode*> CallExpressionSyntax::get_args() const
{
    return _args;
}