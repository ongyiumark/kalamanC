#pragma once

#include "Syntax/syntax.h"

namespace CodeAnalysis
{
    class Evaluator final
    {
    private:
        const ExpressionSyntax* _root;
        int evaluate_expression(const ExpressionSyntax* node) const;
    public:
        Evaluator(const ExpressionSyntax* root);
        int evaluate() const;
    };
}