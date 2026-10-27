#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;

// Defines a function. This behaves more like a python function.
FuncDefineExpressionSyntax::FuncDefineExpressionSyntax(SyntaxToken identifier, std::vector<SyntaxToken>& arg_names, 
    const std::vector<Objects::Type>& arg_types, SyntaxNode* body, Position pos)
    : SyntaxNode(pos), _identifier(identifier), _arg_names(arg_names), _arg_types(arg_types), _body(body) {}

FuncDefineExpressionSyntax::~FuncDefineExpressionSyntax() {}

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

const std::vector<Objects::Type>& FuncDefineExpressionSyntax::get_arg_types() const
{
    return _arg_types;
}

SyntaxToken* FuncDefineExpressionSyntax::get_identifier()
{
    return &_identifier;
}

SyntaxNode* FuncDefineExpressionSyntax::get_body()
{
    return _body.get();
}

std::shared_ptr<SyntaxNode> FuncDefineExpressionSyntax::get_body_shared()
{
    return _body;
}
