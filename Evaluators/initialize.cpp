#include "initialize.h"
#include "../constants.h"
#include <iostream>
#include <algorithm>

using Evaluators::Evaluator;

Contexts::Context context("<program>", nullptr);

// Create Builtin (registers both English and Filipino names)
static void add_builtin_function(const std::string& name, const std::vector<std::string>& arg_names,
                                 int min_args = -1, int max_args = -1)
{
    Objects::Object* func = new Objects::Function(name, arg_names, nullptr, true, min_args, max_args);
    context.get_symbol_table()->set_object(name, func);
}

static void add_bilingual_builtin(const std::string& en_name, const std::string& fil_name,
                                  const std::vector<std::string>& arg_names,
                                  int min_args = -1, int max_args = -1)
{
    add_builtin_function(en_name, arg_names, min_args, max_args);
    if (fil_name != en_name)
        add_builtin_function(fil_name, arg_names, min_args, max_args);
}

void Evaluators::initialize()
{
    add_bilingual_builtin(BI_PRINT,      BI_PRINT_FIL,      {"value"}, 0, -1);
    add_bilingual_builtin(BI_INPUT,      BI_INPUT_FIL,      {},        0, 1);
    add_bilingual_builtin(BI_SPLIT,      BI_SPLIT_FIL,      {"value", "delimiter"}, 1, 2);
    add_bilingual_builtin(BI_SIZE,       BI_SIZE_FIL,       {"value"}, 1, 1);
    add_builtin_function(BI_LEN,                            {"value"}, 1, 1);
    add_bilingual_builtin(BI_TYPE,       BI_TYPE_FIL,       {"value"}, 1, 1);
    add_bilingual_builtin(BI_TO_BOOL,    BI_TO_BOOL_FIL,    {"value"}, 1, 1);
    add_bilingual_builtin(BI_TO_INT,     BI_TO_INT_FIL,     {"value"}, 1, 1);
    add_bilingual_builtin(BI_TO_DOUBLE,  BI_TO_DOUBLE_FIL,  {"value"}, 1, 1);
    add_bilingual_builtin(BI_TO_STRING,  BI_TO_STRING_FIL,  {"value"}, 1, 1);
    add_bilingual_builtin(BI_SET_INDEX,  BI_SET_INDEX_FIL,  {"collection", "index", "value"}, 3, 3);
    add_bilingual_builtin(BI_PUSH,       BI_PUSH_FIL,       {"collection", "value"}, 2, 2);
    add_builtin_function(BI_APPEND,                         {"collection", "value"}, 2, 2);
    add_bilingual_builtin(BI_POP,        BI_POP_FIL,        {"collection", "index"}, 1, 2);
    add_bilingual_builtin(BI_RANGE,      BI_RANGE_FIL,      {"start", "stop", "step"}, 1, 3);
    add_bilingual_builtin(BI_SORT,       BI_SORT_FIL,       {"collection"}, 1, 1);
    add_bilingual_builtin(BI_REVERSE,    BI_REVERSE_FIL,    {"value"}, 1, 1);
    add_bilingual_builtin(BI_JOIN,       BI_JOIN_FIL,       {"collection", "delimiter"}, 1, 2);
    add_bilingual_builtin(BI_ABS,        BI_ABS_FIL,        {"value"}, 1, 1);
    add_bilingual_builtin(BI_MIN,        BI_MIN_FIL,        {"values"}, 1, -1);
    add_bilingual_builtin(BI_MAX,        BI_MAX_FIL,        {"values"}, 1, -1);
    add_bilingual_builtin(BI_SQRT,       BI_SQRT_FIL,       {"value"}, 1, 1);
    add_bilingual_builtin(BI_POW_MOD,    BI_POW_MOD_FIL,    {"base", "exp", "mod"}, 3, 3);
    add_bilingual_builtin(BI_SET_MOD,    BI_SET_MOD_FIL,    {"mod"}, 1, 1);
    add_bilingual_builtin(BI_SET_RECURSION_LIMIT, BI_SET_RECURSION_LIMIT_FIL, {"limit"}, 1, 1);
    add_bilingual_builtin(BI_GET_RECURSION_LIMIT, BI_GET_RECURSION_LIMIT_FIL, {}, 0, 0);
    add_bilingual_builtin(BI_READ_FILE,  BI_READ_FILE_FIL,  {"path"}, 1, 1);
    add_bilingual_builtin(BI_WRITE_FILE, BI_WRITE_FILE_FIL, {"path", "content"}, 2, 2);
    add_bilingual_builtin(BI_CLOCK,      BI_CLOCK_FIL,      {}, 0, 0);
    add_bilingual_builtin(BI_EXIT,       BI_EXIT_FIL,       {"code"}, 0, 1);

    set_script_args({});
}

void Evaluators::set_script_args(const std::vector<std::string>& args)
{
    std::vector<Objects::Object*> elem_objs;
    elem_objs.reserve(args.size());
    for (const auto& a : args)
    {
        elem_objs.push_back(new Objects::String(a));
    }
    Objects::Object* list_obj = new Objects::List(elem_objs);
    context.get_symbol_table()->set_object(VAR_ARGS, list_obj);
    context.get_symbol_table()->set_object(VAR_ARGS_FIL, list_obj->copy());
}

void Evaluators::reset()
{
    context.get_symbol_table()->clear();
    Objects::List::matrix_mod = 1'000'000'007;
    Evaluator::max_recursion_depth = 1000;
    Objects::Boolean::use_filipino = false;
    Objects::Boolean::lang_locked = false;
    Objects::drain_free_lists();
    initialize();
}

void Evaluators::print_symbols()
{
    const auto& table = context.get_symbol_table()->get_table();
    std::vector<std::pair<std::string, Objects::Object*>> user_vars;
    for (const auto& kv : table)
    {
        if (kv.second->type() == Objects::Type::FUNCTION && ((Objects::Function*)kv.second)->is_built_in())
            continue;
        if (kv.first == VAR_ARGS || kv.first == VAR_ARGS_FIL)
            continue;
        user_vars.push_back(kv);
    }
    std::sort(user_vars.begin(), user_vars.end(), [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
    if (user_vars.empty())
    {
        std::cout << "(no user variables defined)\n" << std::endl;
        return;
    }
    for (const auto& kv : user_vars)
    {
        std::cout << "  " << Objects::type_to_string(kv.second->type()) << " "
                  << kv.first << " = " << kv.second->to_string() << std::endl;
    }
    std::cout << std::endl;
}

bool Evaluators::run(std::string &script, bool show_tree, bool show_return, bool is_shell)
{
    Diagnostics::DiagnosticBag::script = script;
    Syntax::Parser parser(script, show_return);
    Syntax::SyntaxNode* root = parser.parse();

    if (show_tree) Syntax::pretty_print(root);

    Objects::Object* answer = nullptr;
    if (!Diagnostics::DiagnosticBag::size()) 
        answer = Evaluator::evaluate(context, root);

    bool had_errors = Diagnostics::DiagnosticBag::size() > 0;
    Diagnostics::DiagnosticBag::print();

    if (!had_errors && show_return && answer != nullptr && answer->type() != Objects::Type::NONE) 
    {
        std::cout << answer->to_string() << "\n" << std::endl;
    }

    delete answer;
    delete root;
    Diagnostics::DiagnosticBag::clear();
    Evaluator::clear();
    return !had_errors;
}
