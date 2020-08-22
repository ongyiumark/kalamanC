#include "syntax-expressions.h"

using namespace Syntax;

BreakExpressionSyntax::BreakExpressionSyntax() {}

SyntaxKind BreakExpressionSyntax::kind() const
{
    return SyntaxKind::BreakExpression;
}