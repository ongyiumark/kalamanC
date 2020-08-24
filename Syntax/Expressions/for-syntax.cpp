#include "syntax-expressions.h"

using namespace Syntax;

// This is for for-loops. Just your standard C-style for-loop.
ForExpressionSyntax::ForExpressionSyntax(SyntaxNode *init, SyntaxNode* condition, SyntaxNode *update, SyntaxNode* body)
    : _init(init), _condition(condition), _update(update), _body(body) {}

ForExpressionSyntax::~ForExpressionSyntax()
{
    delete _init;
    delete _condition;
    delete _update;
    delete _body;
}

SyntaxKind ForExpressionSyntax::kind() const
{
    return SyntaxKind::ForExpression;
}

SyntaxNode* ForExpressionSyntax::get_condition()
{
    return _condition;
}

SyntaxNode* ForExpressionSyntax::get_body()
{
    return _body;
}

SyntaxNode* ForExpressionSyntax::get_init()
{
    return _init;
}

SyntaxNode* ForExpressionSyntax::get_update()
{
    return _update;
}
