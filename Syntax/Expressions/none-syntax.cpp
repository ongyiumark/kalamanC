#include "syntax-expressions.h"

using namespace Syntax;

// This does nothing.
NoneExpressionSyntax::NoneExpressionSyntax() {}

SyntaxKind NoneExpressionSyntax::kind() const
{
    return SyntaxKind::NoneExpression;
}