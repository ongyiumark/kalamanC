#include "syntax-expressions.h"

using namespace Syntax;

VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxToken* identifier, SyntaxNode* value)
    : _identifier(identifier), _value(value) 
{
    _children = {identifier, value};
}

SyntaxKind VarAssignExpressionSyntax::kind() const
{
    return SyntaxKind::VarAssignExpression;
}

SyntaxNode* VarAssignExpressionSyntax::get_value() const
{
    return _value;
}

SyntaxToken* VarAssignExpressionSyntax::get_identifier() const
{
    return _identifier;
}