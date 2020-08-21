#include "syntax-expressions.h"

using namespace Syntax;

VarAssignExpressionSyntax::VarAssignExpressionSyntax(SyntaxToken* identifier, SyntaxNode* node)
    : _identifier(identifier), _node(node) 
{
    _children = {identifier, node};
}

SyntaxKind VarAssignExpressionSyntax::kind() const
{
    return SyntaxKind::VarAssignExpression;
}

SyntaxNode* VarAssignExpressionSyntax::get_node() const
{
    return _node;
}

SyntaxToken* VarAssignExpressionSyntax::get_identifier() const
{
    return _identifier;
}