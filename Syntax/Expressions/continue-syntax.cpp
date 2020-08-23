#include "syntax-expressions.h"

using namespace Syntax;

// This continues a loop. Just your standard continue keyword.
ContinueExpressionSyntax::ContinueExpressionSyntax() {}

SyntaxKind ContinueExpressionSyntax::kind() const
{
    return SyntaxKind::ContinueExpression;
}