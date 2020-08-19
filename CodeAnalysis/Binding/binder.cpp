#include "binding.h"

using namespace CodeAnalysis;

BoundExpression* Binder::Bind(ExpressionSyntax* syntax)
{
    switch(syntax->get_kind())
    {
        case SyntaxKind::LiteralExpression:
            return bind_literal_expression((LiteralExpressionSyntax*)syntax);
        case SyntaxKind::UnaryExpression:
            return bind_unary_expression((UnaryExpressionSyntax*)syntax);
        case SyntaxKind::BinaryExpression:
            return bind_binary_expression((BinaryExpressionSyntax*)syntax);
        default:
            throw new std::exception(); // change in the future -> undexpected syntax {syntax kind}
    }
}

BoundExpression* Binder::bind_literal_expression(LiteralExpressionSyntax* syntax)
{
    
}

BoundExpression* Binder::bind_unary_expression(UnaryExpressionSyntax* syntax)
{

}

BoundExpression* Binder::bind_binary_expression(BinaryExpressionSyntax* syntax)
{

}

