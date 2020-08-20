#pragma once

#include "Binding/binding.h"

namespace CodeAnalysis
{
    class Evaluator final
    {
    private:
        const BoundExpression* _root;
        std::any evaluate_expression(const BoundExpression* node) const;
    public:
        Evaluator(const BoundExpression* root);
        std::any evaluate() const;
    };
}