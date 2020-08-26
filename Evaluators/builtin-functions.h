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
        static Objects::Object* PRINT(Contexts::Context& context);
        static Objects::Object* INPUT(Contexts::Context& context);
        static Objects::Object* SPLIT(Contexts::Context& context);
        static Objects::Object* SIZE(Contexts::Context& context);
        static Objects::Object* TYPE(Contexts::Context& context);
        static Objects::Object* TO_BOOL(Contexts::Context& context);
        static Objects::Object* TO_INT(Contexts::Context& context);
        static Objects::Object* TO_DOUBLE(Contexts::Context& context);
        static Objects::Object* TO_STRING(Contexts::Context& context);
        static Objects::Object* SET_INDEX(Contexts::Context& context);
        friend class Evaluator; 
    };
}
