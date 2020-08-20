#include "code-analysis.h"

using namespace CodeAnalysis;

Evaluator::Evaluator(const BoundExpression* root)
    : _root(root) {}

int Evaluator::evaluate() const
{
    return evaluate_expression(_root);
}

int Evaluator::evaluate_expression(const BoundExpression* node) const
{
    switch (node->get_kind())
    {
        case BoundNodeKind::LiteralExpression:
        {
            BoundLiteralExpression* l = ((BoundLiteralExpression*)node);
            return std::any_cast<int>(l->get_value());
        }
        case BoundNodeKind::UnaryExpression:
        {
            BoundUnaryExpression* u = ((BoundUnaryExpression*)node);
            int operand = evaluate_expression(u->get_operand());
            switch(u->get_op_kind())
            {
                case BoundUnaryOpKind::Negation:
                    return -operand;
                case BoundUnaryOpKind::Identity:
                    return operand;
                default:
                {
                    std::ostringstream os;
                    os << "ERROR Unexpected unary operator: <" << bound_unaryop_kind_to_string(u->get_op_kind()) << ">";
                    throw os.str();
                }
            }
        }
        case BoundNodeKind::BinaryExpression:
        {
            BoundBinaryExpression* b = ((BoundBinaryExpression*)node);
            int left = evaluate_expression(b->get_left());
            int right = evaluate_expression(b->get_right());

            switch(b->get_op_kind())
            {
                case BoundBinaryOpKind::Addition:
                    return left+right;
                case BoundBinaryOpKind::Subtraction:
                    return left-right;
                case BoundBinaryOpKind::Multiplication:
                    return left*right;
                case BoundBinaryOpKind::Division:
                    if (right == 0) 
                    {
                        std::string error = "ERROR: Division by zero";
                        throw error;
                    }
                    return left/right;
                default:
                {
                    std::ostringstream os;
                    os << "ERROR: Unexpected binary operator: <" << bound_binaryop_kind_to_string(b->get_op_kind()) << ">";
                    throw os.str();
                }
            }
        }
        default:
        {
            std::ostringstream os;
            os << "ERROR: Unexpected node: <" << bound_node_kind_to_string(node->get_kind()) << ">";
            throw os.str();
        }
    }
}