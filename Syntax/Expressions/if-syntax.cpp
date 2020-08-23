#include "syntax-expressions.h"
#include <iostream>
using namespace Syntax;

// This is for conditional statements. Just your standard C-style conditional statements.
IfExpressionSyntax::IfExpressionSyntax(std::vector<SyntaxNode*>& conditions, std::vector<SyntaxNode*>& bodies, 
    SyntaxNode* else_body) : _conditions(conditions), _bodies(bodies), _else_body(else_body) {}

SyntaxKind IfExpressionSyntax::kind() const
{
    return SyntaxKind::IfExpression;
}

SyntaxNode* IfExpressionSyntax::get_condition(int i) const
{
    return _conditions[i];
}

SyntaxNode* IfExpressionSyntax::get_body(int i) const
{
    return _bodies[i];
}

SyntaxNode* IfExpressionSyntax::get_else_body() const
{
    return _else_body;
}

int IfExpressionSyntax::get_size() const
{
    return _conditions.size();
}

std::vector<SyntaxNode*> IfExpressionSyntax::get_conditions() const
{
    return _conditions;
}

std::vector<SyntaxNode*> IfExpressionSyntax::get_bodies() const
{
    return _bodies;
}