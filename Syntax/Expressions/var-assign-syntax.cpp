#include "syntax-expressions.h"

using namespace Syntax;

// Assigns a value to an existing variable.
VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxToken* identifier, SyntaxNode* value)
    : _identifier(identifier), _value(value) {}

// Tokens are already deleted by the parser.
VarAssignExpressionSyntax::~VarAssignExpressionSyntax()
{
    delete _value;
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