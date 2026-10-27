#include "syntax-expressions.h"

using namespace Syntax;
using Diagnostics::Position;

// Declares a variable. Pushes a value into the symbol table.
VarDeclareExpressionSyntax::VarDeclareExpressionSyntax(SyntaxToken var_keyword, SyntaxToken identifier, Position pos)
    : SyntaxNode(pos), _var_keyword(var_keyword), _identifier(identifier), _init_expr(nullptr) {}

VarDeclareExpressionSyntax::VarDeclareExpressionSyntax(SyntaxToken var_keyword, SyntaxToken identifier, SyntaxNode* init_expr, Position pos)
    : SyntaxNode(pos), _var_keyword(var_keyword), _identifier(identifier), _init_expr(init_expr) {}

VarDeclareExpressionSyntax::~VarDeclareExpressionSyntax()
{
    delete _init_expr;
}

SyntaxKind VarDeclareExpressionSyntax::kind() const
{
    return SyntaxKind::VarDeclareExpression;
}

SyntaxToken* VarDeclareExpressionSyntax::get_var_keyword()
{
    return &_var_keyword;
}

SyntaxToken* VarDeclareExpressionSyntax::get_identifier()
{
    return &_identifier;
}

SyntaxNode* VarDeclareExpressionSyntax::get_init_expr()
{
    return _init_expr;
}