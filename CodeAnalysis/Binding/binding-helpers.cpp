#include "binding.h"

std::string CodeAnalysis::bound_unaryop_kind_to_string(const BoundUnaryOpKind& kind)
{
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(kind)
    {
        PROCESS_VAL(BoundUnaryOpKind::Identity);
        PROCESS_VAL(BoundUnaryOpKind::Negation);
        PROCESS_VAL(BoundUnaryOpKind::LogicalNegation);
        PROCESS_VAL(BoundUnaryOpKind::Null);
    }
#undef PROCESS_VAL
    return s;
}

std::string CodeAnalysis::bound_binaryop_kind_to_string(const BoundBinaryOpKind& kind)
{
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(kind)
    {
        PROCESS_VAL(BoundBinaryOpKind::Addition);
        PROCESS_VAL(BoundBinaryOpKind::Subtraction);
        PROCESS_VAL(BoundBinaryOpKind::Multiplication);
        PROCESS_VAL(BoundBinaryOpKind::Division);
        PROCESS_VAL(BoundBinaryOpKind::LogicalAnd);
        PROCESS_VAL(BoundBinaryOpKind::LogicalOr);
        PROCESS_VAL(BoundBinaryOpKind::Null);
    }
#undef PROCESS_VAL
    return s;
}

std::string CodeAnalysis::bound_node_kind_to_string(const BoundNodeKind& kind)
{
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(kind)
    {
        PROCESS_VAL(BoundNodeKind::LiteralExpression);
        PROCESS_VAL(BoundNodeKind::UnaryExpression);
        PROCESS_VAL(BoundNodeKind::BinaryExpression);
    }
#undef PROCESS_VAL
    return s;
}