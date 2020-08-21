#pragma once

#include "position.h"
#include "context.h"
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
        Diagnostic(Position pos, std::string message);
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
        void report_bad_character(Position pos, char c);
        void report_invalid_type(Position pos, Objects::Type type);
        void report_expected_character(Position pos, char c);
    };
}