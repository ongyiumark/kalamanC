#include "object.h"

std::string Objects::type_to_string(Type type)
{
    const char* s = 0;
#define PROCESS_VAL(p) case(p): s = #p; break;
    switch(type)
    {
        PROCESS_VAL(Type::BOOLEAN);
        PROCESS_VAL(Type::INTEGER);
        PROCESS_VAL(Type::DOUBLE);
        PROCESS_VAL(Type::STRING);
        PROCESS_VAL(Type::LIST);
        PROCESS_VAL(Type::FUNCTION);
        PROCESS_VAL(Type::NONE);
    }
#undef PROCESS_VALs
    return s;
}