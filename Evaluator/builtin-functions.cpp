#include "builtin-functions.h"
#include <iostream>

using namespace Objects;
using namespace Diagnostics;
using namespace Contexts;

// Prints to the screen.
Object* BuiltInFunctions::BI_PRINT(Context* context)
{
    Object* obj = context->get_symbol_table()->get_object("value");
    std::cout << obj->to_string() << std::endl;
    return new None();
}

// Reads a line as a string.
Object* BuiltInFunctions::BI_INPUT(Context* context)
{
    std::string val;
    getline(std::cin, val);
    return new String(val);
}