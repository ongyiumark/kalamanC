#include "object.h"

// Helper function to convert the type to a readable string.
std::string Objects::type_to_string(Type type)
{
    switch(type)
    {
        case Type::BOOLEAN:  return "bool";
        case Type::INTEGER:  return "int";
        case Type::DOUBLE:   return "double";
        case Type::STRING:   return "string";
        case Type::LIST:     return "list";
        case Type::FUNCTION: return "function";
        case Type::NONE:     return "none";
    }
    return "unknown";
}
