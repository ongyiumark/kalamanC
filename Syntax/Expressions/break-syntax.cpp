#include "syntax-expressions.h"

using namespace Syntax;

// This breaks a loop. Just your standard break keyword.
BreakExpressionSyntax::BreakExpressionSyntax(Diagnostics::Position pos) : SyntaxNode(pos) {}

BreakExpressionSyntax::~BreakExpressionSyntax() {}

SyntaxKind BreakExpressionSyntax::kind() const
{
    return SyntaxKind::BreakExpression;
}
