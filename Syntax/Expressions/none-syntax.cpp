#include "syntax-expressions.h"

using namespace Syntax;

NoneExpressionSyntax::NoneExpressionSyntax() {}

SyntaxKind NoneExpressionSyntax::kind() const
{
    return SyntaxKind::NoneExpression;
}