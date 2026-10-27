#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;

// Assigns or compound-assigns a value to an existing variable or indexed collection.
VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxToken identifier, SyntaxNode* value, Position pos)
    : SyntaxNode(pos), _identifier(identifier), _op_token(SyntaxKind::EqualsToken, pos, "="), _target(nullptr), _value(value), _is_postfix(false) {}

VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxToken identifier, SyntaxToken op_token, SyntaxNode* value, Position pos, bool is_postfix)
    : SyntaxNode(pos), _identifier(identifier), _op_token(op_token), _target(nullptr), _value(value), _is_postfix(is_postfix) {}

VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxNode* target, SyntaxToken op_token, SyntaxNode* value, Position pos, bool is_postfix)
    : SyntaxNode(pos), _identifier(), _op_token(op_token), _target(target), _value(value), _is_postfix(is_postfix) {}

VarAssignExpressionSyntax::~VarAssignExpressionSyntax()
{
    delete _target;
    delete _value;
}

SyntaxKind VarAssignExpressionSyntax::kind() const
{
    return SyntaxKind::VarAssignExpression;
}

SyntaxNode* VarAssignExpressionSyntax::get_value()
{
    return _value;
}

SyntaxToken* VarAssignExpressionSyntax::get_identifier()
{
    return &_identifier;
}

SyntaxToken* VarAssignExpressionSyntax::get_op_token()
{
    return &_op_token;
}

SyntaxNode* VarAssignExpressionSyntax::get_target()
{
    return _target;
}

bool VarAssignExpressionSyntax::is_postfix() const
{
    return _is_postfix;
}
