#include "syntax-expressions.h"

using namespace Syntax;

VarDeclareExpressionSyntax::VarDeclareExpressionSyntax(SyntaxToken* var_keyword, SyntaxToken* identifier)
    :_type(SyntaxFacts::get_keyword_type(var_keyword->kind())), _identifier(identifier) 
{
    _children= {var_keyword, identifier};
}

SyntaxKind VarDeclareExpressionSyntax::kind() const
{
    return SyntaxKind::VarDeclareExpression;
}

Objects::Type VarDeclareExpressionSyntax::get_type() const
{
    return _type;
}

SyntaxToken* VarDeclareExpressionSyntax::get_identifier() const
{
    return _identifier;
}