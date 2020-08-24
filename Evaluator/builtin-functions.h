#pragma once

#include "../Diagnostics/diagnostic.h"
#include "../Objects/object.h"
#include "../Contexts/context.h"

// Refer to builtin-functions.cpp
class BuiltInFunctions
{
private:
    static Diagnostics::DiagnosticBag* _diagnostics;
    static Objects::Object* BI_PRINT(Contexts::Context& context);
    static Objects::Object* BI_INPUT(Contexts::Context& context);
    static Objects::Object* BI_TO_INT(Contexts::Context& context);
    friend class Evaluator; 
public:
    static void initialize(Contexts::Context& context);
};