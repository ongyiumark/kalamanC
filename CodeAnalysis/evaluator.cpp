#include "code-analysis.h"

using namespace CodeAnalysis;

Evaluator::Evaluator(const ExpressionSyntax* root)
    : _root(root) {}

int Evaluator::evaluate() const
{
    return evaluate_expression(_root);
}

int Evaluator::evaluate_expression(const ExpressionSyntax* node) const
{
    switch (node->get_kind())
    {
        case SyntaxKind::LiteralExpression:
        {
            SyntaxToken* number_token = ((LiteralExpressionSyntax*)node)->get_literal_token();
            std::any val = number_token->get_value();
            return *std::any_cast<int>(&val);
            
        }
        case SyntaxKind::UnaryExpression:
        {
            UnaryExpressionSyntax* u = ((UnaryExpressionSyntax*)node);
            int operand = evaluate_expression(u->get_operand());
            switch(u->get_op_token()->get_kind())
            {
                case SyntaxKind::MinusToken:
                    return -operand;
                case SyntaxKind::PlusToken:
                    return operand;
                default:
                {
                    std::ostringstream os;
                    os << "ERROR Unexpected unary operator: <" << syntax_kind_to_string(u->get_op_token()->get_kind()) << ">";
                    throw os.str();
                }
            }
        }
        case SyntaxKind::BinaryExpression:
        {
            BinaryExpressionSyntax* b = ((BinaryExpressionSyntax*)node);
            int left = evaluate_expression(b->get_left());
            int right = evaluate_expression(b->get_right());

            switch(b->get_op_token()->get_kind())
            {
                case SyntaxKind::PlusToken:
                    return left+right;
                case SyntaxKind::MinusToken:
                    return left-right;
                case SyntaxKind::StarToken:
                    return left*right;
                case SyntaxKind::SlashToken:
                    if (right == 0) 
                    {
                        std::string error = "ERROR: Division by zero";
                        throw error;
                    }
                    return left/right;
                default:
                {
                    std::ostringstream os;
                    os << "ERROR: Unexpected binary operator: <" << syntax_kind_to_string(b->get_op_token()->get_kind()) << ">";
                    throw os.str();
                }
            }
        }
        case SyntaxKind::ParenExpression:
        {
            ParenExpressionSyntax* p = ((ParenExpressionSyntax*)node);
            return evaluate_expression(p->get_expression());
        }
        default:
        {
            std::ostringstream os;
            os << "ERROR: Unexpected node: <" << syntax_kind_to_string(node->get_kind()) << ">";
            throw os.str();
        }
    }
}