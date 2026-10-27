#include "object.h"
#include <new>

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

// In-place self-doubling string repetition with zero auxiliary buffer overhead.
Objects::Object* Objects::repeat_string(const std::string& str, long long count)
{
    if (count < 0) return new None();
    if (count == 0 || str.empty()) return new String("");
    if (count == 1) return new String(str);

    // Overflow protection: ensure count * str.size() does not exceed max_size()
    if ((size_t)count > std::string().max_size() / str.size())
        return new None();

    size_t total_len = str.size() * (size_t)count;
    try
    {
        std::string result;
        result.reserve(total_len);
        result = str;
        while (result.size() * 2 <= total_len)
        {
            result.append(result);
        }
        if (result.size() < total_len)
        {
            result.append(result, 0, total_len - result.size());
        }
        return new String(result);
    }
    catch (const std::bad_alloc&)
    {
        return new None();
    }
}
