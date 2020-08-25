#pragma once

#include "../Diagnostics/diagnostic.h"
#include "../Objects/object.h"
#include "../Contexts/context.h"

namespace Evaluators
{
    // Refer to builtin-functions.cpp
    class BuiltInFunctions
    {
    private:
        static Objects::Object* BI_PRINT(Contexts::Context& context);
        static Objects::Object* BI_INPUT(Contexts::Context& context);
        static Objects::Object* BI_TO_INT(Contexts::Context& context);
        friend class Evaluator; 
    };
}
