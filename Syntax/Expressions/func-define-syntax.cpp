#include "syntax-expressions.h"

using namespace Syntax;

// Defines a function. This behaves more like a python function.
FuncDefineExpressionSyntax::FuncDefineExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxToken>& arg_names, SyntaxNode* body)
    : _identifier(identifier), _arg_names(arg_names), _body(body) {}

FuncDefineExpressionSyntax::~FuncDefineExpressionSyntax()
{
    delete _body;
}

SyntaxKind FuncDefineExpressionSyntax::kind() const
{
    return SyntaxKind::FuncDefineExpression;
}

int FuncDefineExpressionSyntax::get_arg_size()
{
    return _arg_names.size();
}

SyntaxToken* FuncDefineExpressionSyntax::get_arg_name(int i) 
{
    return &_arg_names[i];
}

std::vector<SyntaxToken> FuncDefineExpressionSyntax::get_arg_names()
{
    return _arg_names;
}

SyntaxToken* FuncDefineExpressionSyntax::get_identifier()
{
    return &_identifier;
}

SyntaxNode* FuncDefineExpressionSyntax::get_body()
{
    return _body;
}