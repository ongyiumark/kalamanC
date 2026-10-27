#include "builtin-functions.h"
#include "evaluator.h"

#include "../constants.h"
#include "../Syntax/Expressions/syntax-expressions.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstdlib>

using namespace Objects;
using namespace Diagnostics;
using namespace Contexts;
using namespace Evaluators;
using namespace Syntax;

static const auto PROGRAM_START_TIME = std::chrono::steady_clock::now();

// Prints values to the screen (space-separated, without outer quotes on top-level strings).
Object* BuiltInFunctions::PRINT(const std::vector<Object*>& args, Position pos)
{
    int n = args.size();
    for (int i = 0; i < n; i++)
    {
        if (i > 0) std::cout << " ";
        std::cout << args[i]->to_display_string();
    }
    std::cout << std::endl;
    return new None();
}

// Reads a line as a string (with optional prompt).
Object* BuiltInFunctions::INPUT(const std::vector<Object*>& args, Position pos)
{
    if (!args.empty())
    {
        std::cout << args[0]->to_display_string();
        std::cout.flush();
    }
    std::string val;
    std::getline(std::cin, val);
    return new String(val);
}

// Splits the string into a list of words
Object* BuiltInFunctions::SPLIT(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    if (obj->type() != Type::STRING)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_SPLIT, 1, type_to_string(obj->type()), pos);
        return new None();
    }

    std::string sep = " ";
    if (args.size() >= 2)
    {
        Object* delimiter = args[1];
        if (delimiter->type() != Type::STRING)
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_SPLIT, 2, type_to_string(delimiter->type()), pos);
            return new None();
        }
        sep = ((String*)delimiter)->get_value();
        if (sep.empty()) sep = " ";
    }

    std::string words = ((String*)obj)->get_value();
    std::vector<Object*> elems;
    size_t idx = 0;
    while ((idx = words.find(sep)) != std::string::npos)
    {
        if (idx != 0) elems.push_back(new String(words.substr(0, idx)));
        words.erase(0, idx + sep.size());
    }
    if (!words.empty()) elems.push_back(new String(words));
    return new List(elems);
}

// Returns the size of the list or string.
Object* BuiltInFunctions::SIZE(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    switch(obj->type())
    {
        case Type::STRING:
            return new Integer(((String*)obj)->get_size());
        case Type::LIST:
            return new Integer(((List*)obj)->get_size());
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_SIZE, 1, type_to_string(obj->type()), pos);
            return new None();        
    }
}

// Returns the type of the value
Object* BuiltInFunctions::TYPE(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    return new String(type_to_string(obj->type()));
}

// Converts the value to a bool.
Object* BuiltInFunctions::TO_BOOL(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    switch(obj->type())
    {
        case Type::BOOLEAN:
            return new Boolean(((Boolean*)obj)->get_value());
        case Type::INTEGER:
            return new Boolean(((Integer*)obj)->get_value() != 0);
        case Type::DOUBLE:
            return new Boolean(((Double*)obj)->get_value() != 0);
        case Type::STRING:
            return new Boolean(!((String*)obj)->get_value().empty());
        case Type::LIST:
            return new Boolean(((List*)obj)->get_size() != 0);
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_TO_BOOL, 1, type_to_string(obj->type()), pos);
            return new None();        
    }
}

// Converts the value to an integer.
Object* BuiltInFunctions::TO_INT(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    switch (obj->type())
    {
        case Type::STRING:
        {
            std::string text = ((String*)obj)->get_value();
            // Trim leading/trailing whitespace
            size_t first = text.find_first_not_of(" \t\r\n");
            if (first == std::string::npos)
            {
                DiagnosticBag::report_invalid_type(text, type_to_string(Type::INTEGER), pos);
                return new None();
            }
            size_t last = text.find_last_not_of(" \t\r\n");
            std::string trimmed = text.substr(first, last - first + 1);

            bool is_valid = !trimmed.empty();
            size_t start_idx = 0;
            if (trimmed[0] == '-' || trimmed[0] == '+')
            {
                start_idx = 1;
                if (trimmed.size() == 1) is_valid = false;
            }
            for (size_t i = start_idx; i < trimmed.size(); i++)
                is_valid &= is_digit(trimmed[i]);

            std::istringstream is(trimmed);
            long long x;
            if (is_valid && (is >> x)) 
                return new Integer(x);

            DiagnosticBag::report_invalid_type(text, type_to_string(Type::INTEGER), pos);
            return new None();
        }
        case Type::INTEGER:
            return obj->copy();
        case Type::BOOLEAN:
            return new Integer(((Boolean*)obj)->get_value());
        case Type::DOUBLE:
            return new Integer((long long)((Double*)obj)->get_value());
        default:
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_TO_INT, 1, type_to_string(obj->type()), pos);
            return new None();
        }
    }
}

// Converts the value to a double.
Object* BuiltInFunctions::TO_DOUBLE(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    switch (obj->type())
    {
        case Type::STRING:
        {
            std::string text = ((String*)obj)->get_value();
            size_t first = text.find_first_not_of(" \t\r\n");
            if (first == std::string::npos)
            {
                DiagnosticBag::report_invalid_type(text, type_to_string(Type::DOUBLE), pos);
                return new None();
            }
            size_t last = text.find_last_not_of(" \t\r\n");
            std::string trimmed = text.substr(first, last - first + 1);

            bool is_valid = !trimmed.empty();
            int dot_count = 0;
            int digit_count = 0;
            size_t start_idx = 0;
            if (trimmed[0] == '-' || trimmed[0] == '+')
                start_idx = 1;

            for (size_t i = start_idx; i < trimmed.size(); i++)
            {
                char c = trimmed[i];
                if (c == '.') 
                {
                    dot_count++;
                    continue;
                }
                if (dot_count > 1) break;
                if (is_digit(c)) digit_count++;
                else is_valid = false;
            }
            is_valid &= (dot_count <= 1) && (digit_count > 0);

            std::istringstream is(trimmed);
            long double x;
            if (is_valid && (is >> x)) 
                return new Double(x);

            DiagnosticBag::report_invalid_type(text, type_to_string(Type::DOUBLE), pos);
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
            DiagnosticBag::report_invalid_builtin_arguments(BI_TO_DOUBLE, 1, type_to_string(obj->type()), pos);
            return new None();
        }
    }
}

// Converts the value to a string.
Object* BuiltInFunctions::TO_STRING(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    if (obj->type() == Type::STRING) return obj->copy(); 
    return new String(obj->to_string());
}

// Changes the value of a list index (returns a new list copy).
Object* BuiltInFunctions::SET_INDEX(const std::vector<Object*>& args, Position pos)
{
    Object* collection = args[0];
    Object* index = args[1];
    Object* value = args[2];
    switch (collection->type())
    {
        case Type::LIST:
        {
            List* list = (List*)(collection->copy());
            if (index->type() != Type::INTEGER)
            {
                DiagnosticBag::report_invalid_builtin_arguments(BI_SET_INDEX, 2, type_to_string(index->type()), pos);
                delete list;
                return new None();     
            }
            long long idx = ((Integer*)index)->get_value();
            int n = list->get_size();
            if (idx < 0) idx += n;
            if (idx < 0 || idx >= n) 
            {
                DiagnosticBag::report_runtime_error("list index out of range", pos);
                delete list;
                return new None();
            }
            list->set_value((int)idx, value->copy());
            return list;
        }
        default:
            DiagnosticBag::report_invalid_builtin_arguments(BI_SET_INDEX, 1, type_to_string(collection->type()), pos);
            return new None();            
    }
}

// Resolves the underlying List target for in-place mutation (handles both direct variables and nested index chains).
static List* resolve_target_list(Context& caller_ctx, SyntaxNode* first_arg_node)
{
    if (!first_arg_node) return nullptr;

    if (first_arg_node->kind() == SyntaxKind::VarAccessExpression)
    {
        std::string var_name = ((VarAccessExpressionSyntax*)first_arg_node)->get_identifier()->get_text();
        ObjectSymbol sym = caller_ctx.get_symbol_table()->get_object(var_name);
        if (sym.object && sym.object->type() == Type::LIST)
            return (List*)sym.object;
        return nullptr;
    }

    if (first_arg_node->kind() == SyntaxKind::IndexExpression)
    {
        std::vector<IndexExpressionSyntax*> index_chain;
        SyntaxNode* curr = first_arg_node;
        while (curr && curr->kind() == SyntaxKind::IndexExpression)
        {
            IndexExpressionSyntax* idx_node = (IndexExpressionSyntax*)curr;
            if (idx_node->is_slice())
                return nullptr;
            index_chain.push_back(idx_node);
            curr = idx_node->get_to_access();
        }

        if (!curr || curr->kind() != SyntaxKind::VarAccessExpression)
            return nullptr;

        VarAccessExpressionSyntax* base_var = (VarAccessExpressionSyntax*)curr;
        ObjectSymbol obj_sym = caller_ctx.get_symbol_table()->get_object(base_var->get_identifier()->get_text());
        if (obj_sym.object == nullptr)
            return nullptr;

        Object* container = obj_sym.object;
        for (int k = (int)index_chain.size() - 1; k >= 0; k--)
        {
            IndexExpressionSyntax* step = index_chain[k];
            Object* idx_obj = Evaluator::evaluate(caller_ctx, step->get_indexer());
            if (!idx_obj || idx_obj->type() != Type::INTEGER || !container || container->type() != Type::LIST)
            {
                delete idx_obj;
                return nullptr;
            }

            List* list = (List*)container;
            long long idx = ((Integer*)idx_obj)->get_value();
            delete idx_obj;
            int n = list->get_size();
            if (idx < 0) idx += n;
            if (idx < 0 || idx >= n)
                return nullptr;

            container = list->get_value((int)idx);
        }

        if (container && container->type() == Type::LIST)
            return (List*)container;
    }

    return nullptr;
}

// Appends an element to a list (mutates variable/collection in-place if passed an lvalue, and returns the updated list).
Object* BuiltInFunctions::PUSH(Context& caller_ctx, const std::vector<Object*>& args, SyntaxNode* first_arg_node, Position pos)
{
    if (args[0]->type() != Type::LIST)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_PUSH, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }

    List* target_list = resolve_target_list(caller_ctx, first_arg_node);
    if (target_list)
    {
        target_list->push(args[1]->copy());
        return target_list->copy();
    }

    List* copy_list = (List*)args[0]->copy();
    copy_list->push(args[1]->copy());
    return copy_list;
}

// Removes and returns an element from a list (default last element; mutates variable/collection in-place).
Object* BuiltInFunctions::POP(Context& caller_ctx, const std::vector<Object*>& args, SyntaxNode* first_arg_node, Position pos)
{
    if (args[0]->type() != Type::LIST)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_POP, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }

    long long idx = -1;
    if (args.size() >= 2)
    {
        if (args[1]->type() != Type::INTEGER)
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_POP, 2, type_to_string(args[1]->type()), pos);
            return new None();
        }
        idx = ((Integer*)args[1])->get_value();
    }

    List* target_list = resolve_target_list(caller_ctx, first_arg_node);
    if (!target_list)
        target_list = (List*)args[0];

    long long n = target_list->get_size();
    long long norm_idx = (idx < 0) ? (idx + n) : idx;
    if (norm_idx < 0 || norm_idx >= n)
    {
        DiagnosticBag::report_runtime_error("pop index out of range", pos);
        return new None();
    }
    return target_list->pop(norm_idx);
}

// Generates a list of integers: range(stop), range(start, stop), or range(start, stop, step).
Object* BuiltInFunctions::RANGE(const std::vector<Object*>& args, Position pos)
{
    for (size_t i = 0; i < args.size(); i++)
    {
        if (args[i]->type() != Type::INTEGER)
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_RANGE, (int)i + 1, type_to_string(args[i]->type()), pos);
            return new None();
        }
    }

    long long start = 0, stop = 0, step = 1;
    if (args.size() == 1)
    {
        stop = ((Integer*)args[0])->get_value();
    }
    else if (args.size() == 2)
    {
        start = ((Integer*)args[0])->get_value();
        stop = ((Integer*)args[1])->get_value();
    }
    else
    {
        start = ((Integer*)args[0])->get_value();
        stop = ((Integer*)args[1])->get_value();
        step = ((Integer*)args[2])->get_value();
        if (step == 0)
        {
            DiagnosticBag::report_runtime_error("range() step argument must not be zero", pos);
            return new None();
        }
    }

    std::vector<Object*> elems;
    if (step > 0 && start < stop)
    {
        elems.reserve((size_t)((stop - start + step - 1) / step));
        for (long long v = start; v < stop; v += step)
            elems.push_back(new Integer(v));
    }
    else if (step < 0 && start > stop)
    {
        elems.reserve((size_t)((start - stop + (-step) - 1) / (-step)));
        for (long long v = start; v > stop; v += step)
            elems.push_back(new Integer(v));
    }
    return new List(elems);
}

// Returns a sorted copy of a list.
Object* BuiltInFunctions::SORT(const std::vector<Object*>& args, Position pos)
{
    if (args[0]->type() != Type::LIST)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_SORT, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }

    List* src = (List*)args[0];
    int n = src->get_size();
    std::vector<Object*> elems;
    elems.reserve(n);
    for (int i = 0; i < n; i++)
        elems.push_back(src->get_value(i)->copy());

    bool invalid_cmp = false;
    std::sort(elems.begin(), elems.end(), [&](Object* a, Object* b) {
        if (invalid_cmp) return false;
        Object* lt = a->less_than(b);
        if (lt->type() != Type::BOOLEAN)
        {
            delete lt;
            invalid_cmp = true;
            return false;
        }
        bool res = ((Boolean*)lt)->get_value();
        delete lt;
        return res;
    });

    if (invalid_cmp)
    {
        for (auto* o : elems) delete o;
        DiagnosticBag::report_runtime_error("cannot sort list with incomparable element types", pos);
        return new None();
    }
    return new List(elems);
}

// Reverses a list or string.
Object* BuiltInFunctions::REVERSE(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    if (obj->type() == Type::LIST)
    {
        List* src = (List*)obj;
        int n = src->get_size();
        std::vector<Object*> elems;
        elems.reserve(n);
        for (int i = n - 1; i >= 0; i--)
            elems.push_back(src->get_value(i)->copy());
        return new List(elems);
    }
    else if (obj->type() == Type::STRING)
    {
        std::string s = ((String*)obj)->get_value();
        std::reverse(s.begin(), s.end());
        return new String(s);
    }
    DiagnosticBag::report_invalid_builtin_arguments(BI_REVERSE, 1, type_to_string(obj->type()), pos);
    return new None();
}

// Joins a list of elements into a string using a delimiter.
Object* BuiltInFunctions::JOIN(const std::vector<Object*>& args, Position pos)
{
    if (args[0]->type() != Type::LIST)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_JOIN, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }
    std::string delim = "";
    if (args.size() >= 2)
    {
        if (args[1]->type() != Type::STRING)
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_JOIN, 2, type_to_string(args[1]->type()), pos);
            return new None();
        }
        delim = ((String*)args[1])->get_value();
    }

    List* list = (List*)args[0];
    int n = list->get_size();
    std::ostringstream os;
    for (int i = 0; i < n; i++)
    {
        if (i > 0) os << delim;
        os << list->get_value(i)->to_display_string();
    }
    return new String(os.str());
}

// Returns the absolute value of an int or double.
Object* BuiltInFunctions::ABS(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    if (obj->type() == Type::INTEGER)
    {
        long long v = ((Integer*)obj)->get_value();
        return new Integer(v < 0 ? -v : v);
    }
    if (obj->type() == Type::DOUBLE)
    {
        long double v = ((Double*)obj)->get_value();
        return new Double(fabsl(v));
    }
    DiagnosticBag::report_invalid_builtin_arguments(BI_ABS, 1, type_to_string(obj->type()), pos);
    return new None();
}

// Returns the minimum of a list or of multiple arguments.
Object* BuiltInFunctions::MIN(const std::vector<Object*>& args, Position pos)
{
    std::vector<Object*> items;
    if (args.size() == 1 && args[0]->type() == Type::LIST)
    {
        items = ((List*)args[0])->get_values();
        if (items.empty())
        {
            DiagnosticBag::report_runtime_error("min() arg is an empty list", pos);
            return new None();
        }
    }
    else
    {
        items = args;
    }

    Object* best = items[0];
    for (size_t i = 1; i < items.size(); i++)
    {
        Object* lt = items[i]->less_than(best);
        if (lt->type() != Type::BOOLEAN)
        {
            delete lt;
            DiagnosticBag::report_runtime_error("cannot compare incompatible types in min()", pos);
            return new None();
        }
        if (((Boolean*)lt)->get_value())
            best = items[i];
        delete lt;
    }
    return best->copy();
}

// Returns the maximum of a list or of multiple arguments.
Object* BuiltInFunctions::MAX(const std::vector<Object*>& args, Position pos)
{
    std::vector<Object*> items;
    if (args.size() == 1 && args[0]->type() == Type::LIST)
    {
        items = ((List*)args[0])->get_values();
        if (items.empty())
        {
            DiagnosticBag::report_runtime_error("max() arg is an empty list", pos);
            return new None();
        }
    }
    else
    {
        items = args;
    }

    Object* best = items[0];
    for (size_t i = 1; i < items.size(); i++)
    {
        Object* gt = items[i]->greater_than(best);
        if (gt->type() != Type::BOOLEAN)
        {
            delete gt;
            DiagnosticBag::report_runtime_error("cannot compare incompatible types in max()", pos);
            return new None();
        }
        if (((Boolean*)gt)->get_value())
            best = items[i];
        delete gt;
    }
    return best->copy();
}

// Returns the square root of a number as a double.
Object* BuiltInFunctions::SQRT(const std::vector<Object*>& args, Position pos)
{
    Object* obj = args[0];
    long double v = 0;
    if (obj->type() == Type::INTEGER)
        v = (long double)((Integer*)obj)->get_value();
    else if (obj->type() == Type::DOUBLE)
        v = ((Double*)obj)->get_value();
    else
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_SQRT, 1, type_to_string(obj->type()), pos);
        return new None();
    }

    if (v < 0)
    {
        DiagnosticBag::report_runtime_error("sqrt() domain error: negative number", pos);
        return new None();
    }
    return new Double(sqrtl(v));
}

// Modular exponentiation: pow_mod(base, exp, mod) -> (base^exp) % mod
Object* BuiltInFunctions::POW_MOD(const std::vector<Object*>& args, Position pos)
{
    for (int i = 0; i < 3; i++)
    {
        if (args[i]->type() != Type::INTEGER)
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_POW_MOD, i + 1, type_to_string(args[i]->type()), pos);
            return new None();
        }
    }
    long long base = ((Integer*)args[0])->get_value();
    long long exp  = ((Integer*)args[1])->get_value();
    long long mod  = ((Integer*)args[2])->get_value();
    if (exp < 0 || mod <= 0)
    {
        DiagnosticBag::report_runtime_error("pow_mod() requires exp >= 0 and mod > 0", pos);
        return new None();
    }
    base %= mod;
    if (base < 0) base += mod;
    long long res = 1 % mod;
    while (exp > 0)
    {
        if (exp & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return new Integer(res);
}

// Sets the modulo used for matrix operations (pass 0 to disable modulo).
Object* BuiltInFunctions::SET_MOD(const std::vector<Object*>& args, Position pos)
{
    if (args[0]->type() != Type::INTEGER)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_SET_MOD, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }
    List::matrix_mod = ((Integer*)args[0])->get_value();
    return new Integer(List::matrix_mod);
}

// Reads entire contents of a file as a string.
Object* BuiltInFunctions::READ_FILE(const std::vector<Object*>& args, Position pos)
{
    if (args[0]->type() != Type::STRING)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_READ_FILE, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }
    std::string path = ((String*)args[0])->get_value();
    std::ifstream file(path);
    if (!file.is_open())
    {
        DiagnosticBag::report_runtime_error("could not open file '" + path + "'", pos);
        return new None();
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    return new String(ss.str());
}

// Writes string content to a file.
Object* BuiltInFunctions::WRITE_FILE(const std::vector<Object*>& args, Position pos)
{
    if (args[0]->type() != Type::STRING)
    {
        DiagnosticBag::report_invalid_builtin_arguments(BI_WRITE_FILE, 1, type_to_string(args[0]->type()), pos);
        return new None();
    }
    std::string path = ((String*)args[0])->get_value();
    std::ofstream file(path);
    if (!file.is_open())
    {
        DiagnosticBag::report_runtime_error("could not write to file '" + path + "'", pos);
        return new None();
    }
    file << args[1]->to_display_string();
    return new Boolean(true);
}

// Returns elapsed time in seconds since program start as a double.
Object* BuiltInFunctions::CLOCK(const std::vector<Object*>& args, Position pos)
{
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<long double> elapsed = now - PROGRAM_START_TIME;
    return new Double(elapsed.count());
}

// Exits the interpreter with an optional exit status code.
Object* BuiltInFunctions::EXIT(const std::vector<Object*>& args, Position pos)
{
    int code = 0;
    if (!args.empty())
    {
        if (args[0]->type() != Type::INTEGER)
        {
            DiagnosticBag::report_invalid_builtin_arguments(BI_EXIT, 1, type_to_string(args[0]->type()), pos);
            return new None();
        }
        code = (int)((Integer*)args[0])->get_value();
    }
    std::exit(code);
    return new None();
}
