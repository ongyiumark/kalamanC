#pragma once

#include <vector>
#include <string>

#include "position.h"

namespace Diagnostics
{
    // Refer to diagnostic.cpp.
    class Diagnostic final
    {
    private:
        std::string _message;
        Position _pos;
        std::vector<std::string> _call_stack;
    public:
        Diagnostic(const std::string& message, Position pos, const std::vector<std::string>& call_stack = {});
        std::string get_message() const;
        Position get_pos() const;
    };
    
    // Refer to diagnostic-bag.cpp.
    class DiagnosticBag final
    {
    private:
        static std::vector<Diagnostic> _diagnostics;
        static std::vector<std::string> _call_stack;
        static void report(const std::string& message, Position pos);
    public:
        DiagnosticBag();
        static std::string script;

        static int size();
        static Diagnostic diagnostic(int i);
        static const std::vector<Diagnostic>& diagnostics();
        static void print();
        static void clear();

        static void push_call(const std::string& func_name, Position call_pos);
        static void pop_call();
        static int call_depth();
        
        static void report_bad_character(char c, Position pos);
        static void report_invalid_type(const std::string& text, const std::string& type, Position pos);
        static void report_expected_character(char c, Position pos);
        static void report_unexpected_token(const std::string& actual, const std::string& expected, Position pos);
        static void report_illegal_binary_operation(const std::string& left, const std::string& operation, const std::string& right, Position pos);
        static void report_illegal_unary_operation(const std::string& operation, const std::string& operand, Position pos);
        static void report_unknown_syntax(const std::string& syntax, Position pos);
        static void report_unexpected_type(const std::string& actual, const std::string& expected, Position pos);
        static void report_invalid_assign(const std::string& actual, const std::string& expected, Position pos);
        static void report_undeclared_identifier(const std::string& identifier, Position pos);
        static void report_illegal_arguments(int actual, int expected, const std::string& name, Position pos);
        static void report_illegal_arguments_range(int actual, int min_expected, int max_expected, const std::string& name, Position pos);
        static void report_unreachable_code(const std::string& info, Position pos);
        static void report_invalid_builtin_arguments(const std::string& name, int i, const std::string& type, Position pos);
        static void report_invalid_break(Position pos);
        static void report_invalid_continue(Position pos);
        static void report_invalid_return(Position pos);
        static void report_redefinition(const std::string& name, Position pos);
        static void report_runtime_error(const std::string& message, Position pos);
    };
}
