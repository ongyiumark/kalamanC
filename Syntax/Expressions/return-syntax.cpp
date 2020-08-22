#include "syntax-expressions.h"

using namespace Syntax;

ReturnExpressionSyntax::ReturnExpressionSyntax(SyntaxNode* to_return)
    :  _to_return(to_return)
{
   if (to_return) _children = {to_return};
}

SyntaxKind ReturnExpressionSyntax::kind() const
{
    return SyntaxKind::ReturnExpression;
}

SyntaxNode* ReturnExpressionSyntax::get_to_return() const
{
    return _to_return;
}