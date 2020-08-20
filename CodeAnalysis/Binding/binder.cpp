#include "binding.h"

using namespace CodeAnalysis;

BoundExpression* Binder::bind_expression(ExpressionSyntax* syntax)
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
    std::any any_val = syntax->get_literal_token()->get_value();
    int * val_ptr = std::any_cast<int>(&any_val);
    int value = (val_ptr ? *val_ptr : 0); 
    return new BoundLiteralExpression(std::make_any<int>(value));
}

BoundExpression* Binder::bind_unary_expression(UnaryExpressionSyntax* syntax)
{
    BoundExpression* operand = bind_expression(syntax->get_operand());
    BoundUnaryOpKind op_kind = bind_unary_operator_kind(syntax->get_op_token()->get_kind());
    return new BoundUnaryExpression(op_kind, operand);
}

BoundExpression* Binder::bind_binary_expression(BinaryExpressionSyntax* syntax)
{
    BoundExpression* left = bind_expression(syntax->get_left());
    BoundBinaryOpKind op_kind = bind_binary_operator_kind(syntax->get_op_token()->get_kind());
    BoundExpression* right = bind_expression(syntax->get_right());
    return new BoundBinaryExpression(left, op_kind, right);
}

BoundUnaryOpKind Binder::bind_unary_operator_kind(SyntaxKind kind)
{
    switch(kind)
    {
        case SyntaxKind::PlusToken:
            return BoundUnaryOpKind::Identity;
        case SyntaxKind::MinusToken:
            return BoundUnaryOpKind::Negation;
        default:
        {
            std::ostringstream os;
            os << "Unexpected unary operator " << syntax_kind_to_string(kind);
            throw os.str();
        }
    }
}

BoundBinaryOpKind Binder::bind_binary_operator_kind(SyntaxKind kind)
{

}


