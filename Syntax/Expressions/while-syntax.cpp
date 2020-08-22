#include "syntax-expressions.h"

using namespace Syntax;

// This is for while-loops. Just your standard while loop.
WhileExpressionSyntax::WhileExpressionSyntax(SyntaxNode *condition, SyntaxNode* body)
    : _condition(condition), _body(body) {}
    
SyntaxKind WhileExpressionSyntax::kind() const
{
    return SyntaxKind::WhileExpression;
}

SyntaxNode* WhileExpressionSyntax::get_condition() const
{
    return _condition;
}

SyntaxNode* WhileExpressionSyntax::get_body() const
{
    return _body;
}
