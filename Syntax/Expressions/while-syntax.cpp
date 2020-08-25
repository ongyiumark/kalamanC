#include "syntax-expressions.h"

using namespace Syntax;

// This is for while-loops. Just your standard while loop.
WhileExpressionSyntax::WhileExpressionSyntax(SyntaxNode *condition, SyntaxNode* body, Diagnostics::Position pos)
    : SyntaxNode(pos), _condition(condition), _body(body) {}
    
WhileExpressionSyntax::~WhileExpressionSyntax()
{
    delete _condition;
    delete _body;
}

SyntaxKind WhileExpressionSyntax::kind() const
{
    return SyntaxKind::WhileExpression;
}

SyntaxNode* WhileExpressionSyntax::get_condition()
{
    return _condition;
}

SyntaxNode* WhileExpressionSyntax::get_body()
{
    return _body;
}
