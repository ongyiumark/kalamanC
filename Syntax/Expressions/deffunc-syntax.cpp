#include "syntax-expressions.h"

using namespace Syntax;

DefFuncExpressionSyntax::DefFuncExpressionSyntax(SyntaxToken* identifier, std::vector<std::string>& arg_names, SyntaxNode* body)
    : _identifier(identifier), _arg_names(arg_names), _body(body) {}

SyntaxKind DefFuncExpressionSyntax::kind() const
{
    return SyntaxKind::DefineExpression;
}

int DefFuncExpressionSyntax::get_arg_size() const
{
    return _arg_names.size();
}

std::string DefFuncExpressionSyntax::get_arg_name(int i) const
{
    return _arg_names[i];
}

std::vector<std::string> DefFuncExpressionSyntax::get_arg_names() const
{
    return _arg_names;
}

SyntaxToken* DefFuncExpressionSyntax::get_identifier() const
{
    return _identifier;
}

SyntaxNode* DefFuncExpressionSyntax::get_body() const
{
    return _body;
}