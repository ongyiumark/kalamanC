#pragma once

#include "../Objects/object.h"
#include <vector>

namespace Diagnostics
{
    // Refer to diagnostic.cpp.
    class Diagnostic final
    {
    private:
        std::string _message;
    public:
        Diagnostic(std::string& message);
        std::string get_message() const;
    };
    
    // Refer to diagnostic-bag.ccp.
    class DiagnosticBag final
    {
    private:
        static std::vector<Diagnostic> _diagnostics;

        static void report(std::string message);
    public:
        DiagnosticBag();

        static bool should_return();
        static bool to_continue;
        static bool to_break;
        static Objects::Object* return_value;

        static int size();
        static Diagnostic diagnostic(int i);
        static std::vector<Diagnostic> diagnostics();
        static void print();
        static void clear();
        
        static void report_bad_character(char c);
        static void report_invalid_type(std::string text, std::string type);
        static void report_expected_character(char c);
        static void report_unexpected_token(std::string actual, std::string expected);
        static void report_illegal_binary_operation(std::string left, std::string operation, std::string right);
        static void report_illegal_unary_operation(std::string operation, std::string operand);
        static void report_unknown_syntax(std::string syntax);
        static void report_unexpected_type(std::string acutal, std::string expected);
        static void report_invalid_assign(std::string actual, std::string expected);
        static void report_undeclared_identifier(std::string identifier);
        static void report_illegal_arguments(int expected, int actual);
        static void report_unreachable_code(std::string info);
        static void report_invalid_builtin_arguments(std::string name, int i, std::string type);
        static void report_uninitialized_function();
    };
}