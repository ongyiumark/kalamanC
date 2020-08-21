#include "syntax-expressions.h"

using namespace Syntax;

ForExpressionSyntax::ForExpressionSyntax(SyntaxNode *init, SyntaxNode* condition, SyntaxNode *update, SyntaxNode* body)
    : _init(init), _condition(condition), _update(update), _body(body) 
{
    _children = {init, condition, update, body};
}

SyntaxKind ForExpressionSyntax::kind() const
{
    return SyntaxKind::ForExpression;
}

SyntaxNode* ForExpressionSyntax::get_condition() const
{
    return _condition;
}

SyntaxNode* ForExpressionSyntax::get_body() const
{
    return _body;
}

SyntaxNode* ForExpressionSyntax::get_init() const
{
    return _init;
}

SyntaxNode* ForExpressionSyntax::get_update() const
{
    return _update;
}
