#include "syntax-expressions.h"

using namespace Syntax;

// This is for for-loops. Supports both C-style for(init; cond; update) and foreach for(x in list).
ForExpressionSyntax::ForExpressionSyntax(SyntaxNode *init, SyntaxNode* condition, SyntaxNode *update, SyntaxNode* body, Diagnostics::Position pos, bool is_foreach)
    : SyntaxNode(pos), _init(init), _condition(condition), _update(update), _body(body), _is_foreach(is_foreach) {}

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

bool ForExpressionSyntax::is_foreach() const
{
    return _is_foreach;
}
