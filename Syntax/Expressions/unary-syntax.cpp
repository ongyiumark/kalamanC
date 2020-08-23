#include "syntax-expressions.h"

using namespace Syntax;

// This is for unary operations.
UnaryExpressionSyntax::UnaryExpressionSyntax(SyntaxToken* op_token, SyntaxNode* operand)
    : _op_token(op_token), _operand(operand) {}
    
// Tokens are already deleted by the parser.
UnaryExpressionSyntax::~UnaryExpressionSyntax()
{
    delete _operand;
}

SyntaxKind UnaryExpressionSyntax::kind() const
{
    return SyntaxKind::UnaryExpression;
}

SyntaxToken* UnaryExpressionSyntax::get_op_token() const
{
    return _op_token;
}

SyntaxNode* UnaryExpressionSyntax::get_operand() const
{
    return _operand;
}