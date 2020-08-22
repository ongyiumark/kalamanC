#pragma once

#include "position.h"
#include "../Objects/object.h"
#include <vector>

namespace Diagnostics
{
    class Diagnostic final
    {
    private:
        Position _pos;
        std::string _message;
    public:
        Diagnostic(Position pos, std::string& message);
        std::string get_message() const;
        Position get_pos() const;
    };

    class DiagnosticBag final
    {
    private:
        static std::vector<Diagnostic> _diagnostics;
        void report(Position pos, std::string message);
    public:
        DiagnosticBag();
        static int size();
        static Diagnostic diagnostic(int i);
        static std::vector<Diagnostic> diagnostics();
        static void print();
        static void clear();
        static bool should_return();
        static bool to_continue;
        static bool to_break;
        static Objects::Object* return_value;
        
        void report_bad_character(Position pos, char c);
        void report_invalid_type(Position pos, std::string type);
        void report_expected_character(Position pos, char c);
        void report_unexpected_token(Position pos, std::string actual, std::string expected);
        void report_illegal_binary_operation(std::string left, std::string operation, std::string right);
        void report_illegal_unary_operation(std::string operation, std::string operand);
        void report_unknown_syntax(std::string syntax);
        void report_expected_type(std::string expected, std::string acutal);
        void report_invalid_assign(std::string actual, std::string expected);
        void report_undeclared_variable(std::string identifier);
        void report_illegal_arguments(int expected, int actual);
    };
}