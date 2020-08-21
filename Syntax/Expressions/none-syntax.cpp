#include "syntax-expressions.h"

using namespace Syntax;

NoneExpressionSyntax::NoneExpressionSyntax(SyntaxToken* semicolon_token)
    : _semicolon_token(semicolon_token)
{
    _children = {semicolon_token};
}

SyntaxKind NoneExpressionSyntax::kind() const
{
    return SyntaxKind::NoneExpression;
}