#pragma once

#include "../Diagnostics/diagnostic.h"
#include "../Objects/object.h"
#include "../Contexts/context.h"
#include <vector>

namespace Evaluators
{
    // Refer to builtin-functions.cpp
    class BuiltInFunctions
    {
    private:
        static Objects::Object* PRINT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* INPUT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SPLIT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SIZE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* TYPE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* TO_BOOL(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* TO_INT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* TO_DOUBLE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* TO_STRING(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SET_INDEX(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* PUSH(Contexts::Context& caller_ctx, const std::vector<Objects::Object*>& args,
                                     Syntax::SyntaxNode* first_arg_node, Diagnostics::Position pos);
        static Objects::Object* POP(Contexts::Context& caller_ctx, const std::vector<Objects::Object*>& args,
                                    Syntax::SyntaxNode* first_arg_node, Diagnostics::Position pos);
        static Objects::Object* RANGE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SORT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* REVERSE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* JOIN(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* ABS(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* MIN(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* MAX(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SQRT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* POW_MOD(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SET_MOD(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* SET_RECURSION_LIMIT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* GET_RECURSION_LIMIT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* READ_FILE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* WRITE_FILE(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* CLOCK(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        static Objects::Object* EXIT(const std::vector<Objects::Object*>& args, Diagnostics::Position pos);
        friend class Evaluator; 
    };
}
