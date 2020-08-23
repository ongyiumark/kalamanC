#include "syntax-expressions.h"

using namespace Syntax;

// This returns a value. Just your standard return keyword.
ReturnExpressionSyntax::ReturnExpressionSyntax(SyntaxNode* to_return) : _to_return(to_return) {}

ReturnExpressionSyntax::~ReturnExpressionSyntax()
{
    delete _to_return;
}

SyntaxKind ReturnExpressionSyntax::kind() const
{
    return SyntaxKind::ReturnExpression;
}

SyntaxNode* ReturnExpressionSyntax::get_to_return() const
{
    return _to_return;
}