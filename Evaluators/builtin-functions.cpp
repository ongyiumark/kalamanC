#include "builtin-functions.h"

#include "../constants.h"
#include "../Syntax/syntax.h"
#include <iostream>

using namespace Objects;
using namespace Diagnostics;
using namespace Contexts;
using namespace Evaluators;
using namespace Syntax;

// Prints to the screen.
Object* BuiltInFunctions::PRINT(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    std::cout << obj->to_string() << std::endl;
    return new None();
}

// Reads a line as a string.
Object* BuiltInFunctions::INPUT(Context& context)
{
    std::string val;
    getline(std::cin, val);
    return new String(val);
}

// Splits the string into a list of words
Object* BuiltInFunctions::SPLIT(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    Object* delimiter = context.get_symbol_table()->get_object("delimiter").object;

    switch(obj->type())
    {
        case Type::STRING:
        {
            if (delimiter->type() != Type::STRING)
            {
                DiagnosticBag::report_invalid_builtin_arguments(BI_SPLIT, 2, type_to_string(delimiter->type()), Position());
                return new None();
            }
            std::string sep = ((String*)delimiter)->get_value();
            if (sep.size() == 0) sep = " ";
            std::string words = ((String*)obj)->get_value();
            std::vector<Object*> elems;
            size_t pos = 0;
            while ((pos = words.find(sep)) != std::string::npos)
            {
                if (pos != 0) elems.push_back(new String(words.substr(0, pos)));
                words.erase(0, pos+sep.size());
            }
            if (words.size() > 0) elems.push_back(new String(words));
            return new List(elems);
        }
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_SPLIT, 1, type_to_string(obj->type()), Position());
            return new None();
    }    
}

// Returns the size of the list or string.
Object* BuiltInFunctions::SIZE(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    switch(obj->type())
    {
        case Type::STRING:
            return new Integer(((String*)obj)->get_size());
        case Type::LIST:
            return new Integer(((List*)obj)->get_size());
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_SIZE, 1, type_to_string(obj->type()), Position());
            return new None();        
    }
}

// Returns the type of the value
Object* BuiltInFunctions::TYPE(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    return new String(type_to_string(obj->type()));
}

// Converts the value to a bool.
Object* BuiltInFunctions::TO_BOOL(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    switch(obj->type())
    {
        case Type::BOOLEAN:
            return new Boolean(((Boolean*)obj)->get_value());
        case Type::INTEGER:
            return new Boolean(((Integer*)obj)->get_value() != 0);
        case Type::DOUBLE:
            return new Boolean(((Double*)obj)->get_value() != 0);
        case Type::STRING:
            return new Boolean(((String*)obj)->get_value() != "");
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_TO_BOOL, 1, type_to_string(obj->type()), Position());
            return new None();        
    }
}

// Converts the value to an integer.
Object* BuiltInFunctions::TO_INT(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    switch (obj->type())
    {
        case Type::STRING:
        {
            std::string text = ((String*)obj)->get_value();

            bool is_valid = true;
            for (char c : text)
                is_valid &= is_digit(c);


            std::istringstream is(text);
            long long x;
            if (is >> x && is_valid) 
                return  new Integer(x);

            DiagnosticBag::report_invalid_type(text, type_to_string(Type::INTEGER), Position());
            return new None();
        }
        case Type::INTEGER:
            return obj->copy();
        case Type::BOOLEAN:
            return new Integer(((Boolean*)obj)->get_value());
        case Type::DOUBLE:
            return new Integer(((Double*)obj)->get_value());
        default:
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_TO_INT, 1, type_to_string(obj->type()), Position());
            return new None();
        }
    }
}

// Converts the value to a double.
Object* BuiltInFunctions::TO_DOUBLE(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    switch (obj->type())
    {
        case Type::STRING:
        {
            std::string text = ((String*)obj)->get_value();

            bool is_valid = true;
            int dot_count = 0;
            for (char &c : text)
            {
                if (c == '.') 
                {
                    dot_count++;
                    continue;
                }
                if (dot_count > 1) break;
                is_valid &= is_digit(c);
            }
            is_valid &= (dot_count <= 1);

            std::istringstream is(text);
            long double x;
            if (is >> x && is_valid) 
                return new Double(x);

            DiagnosticBag::report_invalid_type(text, type_to_string(Type::DOUBLE), Position());
            return new None();
        }
        case Type::INTEGER:
            return new Double(((Integer*)obj)->get_value());
        case Type::BOOLEAN:
            return new Double(((Boolean*)obj)->get_value());
        case Type::DOUBLE:
            return obj->copy();
        default:
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_TO_DOUBLE, 1, type_to_string(obj->type()), Position());
            return new None();
        }
    }
}

// Converts the value to a string.
Object* BuiltInFunctions::TO_STRING(Context& context)
{
    Object* obj = context.get_symbol_table()->get_object("value").object;
    if (obj->type() == Type::STRING) return obj->copy(); 
    return new String(obj->to_string());
}

// Changes the value of a list index.
Object* BuiltInFunctions::SET_INDEX(Context& context)
{
    Object* collection = context.get_symbol_table()->get_object("collection").object;
    Object* index = context.get_symbol_table()->get_object("index").object;
    Object* value = context.get_symbol_table()->get_object("value").object;
    switch (collection->type())
    {
        case Type::LIST:
        {
            List* list = (List*)(collection->copy());
            if (index->type() != Type::INTEGER)
            {
                DiagnosticBag::report_invalid_builtin_arguments(BI_SET_INDEX, 2, type_to_string(index->type()), Position());
                delete list;
                return new None();     
            }
            long long idx = ((Integer*)index)->get_value();
            int n = list->get_size();
            if (idx < 0) idx += n;
            if (idx < 0 || idx >= n) 
            {
                DiagnosticBag::report_illegal_binary_operation(type_to_string(list->type()),
                kind_to_string(SyntaxKind::IndexExpression), type_to_string(index->type()), Position());
                delete list;
                return new None();
            }
            list->set_value(idx, value->copy());
            return list;
        }
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_SET_INDEX, 1, type_to_string(collection->type()), Position());
            return new None();            
    }
}