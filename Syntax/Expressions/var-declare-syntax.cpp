#include "syntax-expressions.h"

using namespace Syntax;

VarDeclareExpressionSyntax::VarDeclareExpressionSyntax(SyntaxToken* var_keyword, SyntaxToken* identifier)
    : _var_keyword(var_keyword), _identifier(identifier) 
{
    _children= {var_keyword, identifier};
}

SyntaxKind VarDeclareExpressionSyntax::kind() const
{
    return SyntaxKind::VarDeclareExpression;
}

SyntaxToken* VarDeclareExpressionSyntax::get_var_keyword() const
{
    return _var_keyword;
}

SyntaxToken* VarDeclareExpressionSyntax::get_identifier() const
{
    return _identifier;
}