#include "syntax-expressions.h"

using namespace Syntax;

// This is for conditional statements. Just your standard C-style conditional statements.
IfExpressionSyntax::IfExpressionSyntax(std::vector<SyntaxNode*>& conditions, std::vector<SyntaxNode*>& bodies, 
    SyntaxNode* else_body) : _conditions(conditions), _bodies(bodies), _else_body(else_body) {}

IfExpressionSyntax::~IfExpressionSyntax()
{
    for (auto &o : _conditions)
        delete o;
    
    for (auto &o : _bodies)
        delete o;
    
    delete _else_body;
}

SyntaxKind IfExpressionSyntax::kind() const
{
    return SyntaxKind::IfExpression;
}

SyntaxNode* IfExpressionSyntax::get_condition(int i)
{
    return _conditions[i];
}

SyntaxNode* IfExpressionSyntax::get_body(int i)
{
    return _bodies[i];
}

SyntaxNode* IfExpressionSyntax::get_else_body()
{
    return _else_body;
}

int IfExpressionSyntax::get_size()
{
    return _conditions.size();
}

std::vector<SyntaxNode*> IfExpressionSyntax::get_conditions()
{
    return _conditions;
}

std::vector<SyntaxNode*> IfExpressionSyntax::get_bodies()
{
    return _bodies;
}