#pragma once

#include "Binding/binding.h"

namespace CodeAnalysis
{
    class Evaluator final
    {
    private:
        const BoundExpression* _root;
        int evaluate_expression(const BoundExpression* node) const;
    public:
        Evaluator(const BoundExpression* root);
        int evaluate() const;
    };
}