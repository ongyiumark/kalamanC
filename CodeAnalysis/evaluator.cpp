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
        case SyntaxKind::NumberExpression:
        {
            SyntaxToken* number_token = ((LiteralExpressionSyntax*)node)->get_literal_token();
            return std::any_cast<int>(number_token->get_value());
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
                    if (right == 0) throw new std::exception(); // change in the future -> division by zero
                    return left/right;
                default:
                {
                    throw new std::exception(); // change in the future -> unexpected operator
                }
            }
        }
        case SyntaxKind::ParenExpression:
        {
            ParenExpressionSyntax* p = ((ParenExpressionSyntax*)node);
            return evaluate_expression(p->get_expression());
        }
        default:
            throw new std::exception(); // change in the future -> unxpected node 
            break;
    }
}