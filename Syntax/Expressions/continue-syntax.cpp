#include "syntax-expressions.h"

using namespace Syntax;

ContinueExpressionSyntax::ContinueExpressionSyntax() {}

SyntaxKind ContinueExpressionSyntax::kind() const
{
    return SyntaxKind::ContinueExpression;
}