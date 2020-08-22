#include "syntax-expressions.h"
#include <iostream>
using namespace Syntax;

IfExpressionSyntax::IfExpressionSyntax(std::vector<SyntaxNode*>& conditions, std::vector<SyntaxNode*>& bodies, 
    SyntaxNode* else_body)
    : _conditions(conditions), _bodies(bodies), _else_body(else_body)
{
    int n = conditions.size();
    std::cout << n << std::endl;
    
    SyntaxToken* if_keyword = new SyntaxToken(SyntaxKind::IfKeyword, -1, "\0", NULL);
    SyntaxToken* elif_keyword = new SyntaxToken(SyntaxKind::ElifKeyword, -1, "\0", NULL);
    
    for (int i = 0; i < n; i++)
    {
        if (i == 0) _children.push_back(if_keyword);
        else _children.push_back(elif_keyword);
        _children.push_back(conditions[i]);
        _children.push_back(bodies[i]);
    }
    if (else_body) 
    {
        _children.push_back(new SyntaxToken(SyntaxKind::ElseKeyword, -1, "\0", NULL));
        _children.push_back(else_body);
    }
}

SyntaxKind IfExpressionSyntax::kind() const
{
    return SyntaxKind::IfExpression;
}

SyntaxNode* IfExpressionSyntax::get_condition(int i) const
{
    return _conditions[i];
}

SyntaxNode* IfExpressionSyntax::get_expression(int i) const
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