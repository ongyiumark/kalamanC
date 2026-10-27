#include "diagnostic.h"

#include <sstream>
#include <iostream>

using namespace Diagnostics;

// The current script that's being interpreted.
std::string DiagnosticBag::script = "";

// I made this static so I just have one bag for the entire program.
std::vector<Diagnostic> DiagnosticBag::_diagnostics = std::vector<Diagnostic>(); 
std::vector<std::string> DiagnosticBag::_call_stack = std::vector<std::string>();

DiagnosticBag::DiagnosticBag() {}

int DiagnosticBag::size()
{
    return _diagnostics.size();
}

Diagnostic DiagnosticBag::diagnostic(int i)
{
    return _diagnostics[i];
}

const std::vector<Diagnostic>& DiagnosticBag::diagnostics()
{
    return _diagnostics;
}

void DiagnosticBag::push_call(const std::string& func_name, Position call_pos)
{
    std::ostringstream os;
    os << func_name << "()";
    if (call_pos.ln != -1)
        os << " (line " << call_pos.ln + 1 << ", col " << call_pos.col + 1 << ")";
    _call_stack.push_back(os.str());
}

void DiagnosticBag::pop_call()
{
    if (!_call_stack.empty())
        _call_stack.pop_back();
}

int DiagnosticBag::call_depth()
{
    return (int)_call_stack.size();
}

// Adds an error to the bag.
void DiagnosticBag::report(const std::string& message, Position pos)
{
    _diagnostics.emplace_back(message, pos, _call_stack);
}

// Prints all the errors.
void DiagnosticBag::print()
{
    int n = _diagnostics.size();
    for (int i = 0; i < n; i++)
        std::cout << _diagnostics[i].get_message() << std::endl;
    if (n) std::cout << std::endl;
}

// Resets everything.
void DiagnosticBag::clear()
{
    _diagnostics.clear();
    _call_stack.clear();
}

// Occurs when the lexer encounters an unknown character.
void DiagnosticBag::report_bad_character(char c, Position pos)
{
    std::ostringstream os;
    os << "ERROR: bad character '" << c << "'";
    report(os.str(), pos);
}

// Occurs when parsing to an integer or double fails.
void DiagnosticBag::report_invalid_type(const std::string& text, const std::string& type, Position pos)
{
    std::ostringstream os;
    os << "ERROR: '" << text << "' is not a valid " << type;
    report(os.str(), pos);
}

// Occurs when the lexer fails to see an expected character.
void DiagnosticBag::report_expected_character(char c, Position pos)
{
    std::ostringstream os;
    os << "ERROR: expected character '" << c << "'";
    report(os.str(), pos);
}

// Occurs when the parser encounters a different token than expected.
void DiagnosticBag::report_unexpected_token(const std::string& actual, const std::string& expected, Position pos)
{
    std::ostringstream os;
    os << "ERROR: unexpected token <" << actual << ">, ";
    os << "expected <" << expected << ">";
    report(os.str(), pos);
}

// Occurs when a unary operation fails.
void DiagnosticBag::report_illegal_unary_operation(const std::string& operation, const std::string& operand, Position pos)
{
    std::ostringstream os;
    os << "ERROR: illegal operation <" << operation << "> for <" << operand << ">"; 
    report(os.str(), pos);
}

// Occurs when a binary operation fails.
void DiagnosticBag::report_illegal_binary_operation(const std::string& left, const std::string& operation, const std::string& right, Position pos)
{
    std::ostringstream os;
    os << "ERROR: illegal operation <" << operation << "> between <" << left << ">"; 
    os << " and <" << right << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator encounters an unknown syntax.
// Realistically this should never occur unless I messed up.
void DiagnosticBag::report_unknown_syntax(const std::string& syntax, Position pos)
{
    std::ostringstream os;
    os << "ERROR: unknown syntax <" << syntax << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator encounters a different type than expected.
void DiagnosticBag::report_unexpected_type(const std::string& actual, const std::string& expected, Position pos)
{
    std::ostringstream os;
    os << "ERROR: unexpected type <" << actual << ">, ";
    os << "expected <" << expected << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator tries to assign a value to a variable of a different type.
void DiagnosticBag::report_invalid_assign(const std::string& actual, const std::string& expected, Position pos)
{
    std::ostringstream os;
    os << "ERROR: invalid assignment of <" << actual << "> ";
    os << "to <" << expected << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator tries to access an undeclared identifier.
void DiagnosticBag::report_undeclared_identifier(const std::string& identifier, Position pos)
{
    std::ostringstream os;
    os << "ERROR: undeclared identifier '" << identifier << "'";
    report(os.str(), pos);
}

// Occurs when the wrong number of arguments are given when calling a function.
void DiagnosticBag::report_illegal_arguments(int actual, int expected, const std::string& name, Position pos)
{
    std::ostringstream os;
    os << "ERROR: function '" << name << "' provided with " << actual << " argument(s), expected " << expected;
    report(os.str(), pos);    
}

void DiagnosticBag::report_illegal_arguments_range(int actual, int min_expected, int max_expected, const std::string& name, Position pos)
{
    std::ostringstream os;
    if (min_expected == max_expected)
    {
        os << "ERROR: function '" << name << "' provided with " << actual << " argument(s), expected " << min_expected;
    }
    else if (max_expected == -1)
    {
        os << "ERROR: function '" << name << "' provided with " << actual << " argument(s), expected at least " << min_expected;
    }
    else
    {
        os << "ERROR: function '" << name << "' provided with " << actual << " argument(s), expected " << min_expected << " to " << max_expected;
    }
    report(os.str(), pos);
}

// Occurs when supposedly unreachable code is reached.
// Again, this should never occur unless I messed up.
void DiagnosticBag::report_unreachable_code(const std::string& info, Position pos)
{
    std::ostringstream os;
    os << "ERROR: reached unreachable code, " << info;
    report(os.str(), pos);    
}

// Occurs when invalid arguments are passed to a builtin function.
void DiagnosticBag::report_invalid_builtin_arguments(const std::string& name, int i, const std::string& type, Position pos)
{
    std::ostringstream os;
    os << "ERROR: argument " << i << " of '" << name << "' cannot be <" << type << ">";
    report(os.str(), pos);    
}

void DiagnosticBag::report_invalid_break(Position pos)
{
    report("ERROR: 'break' outside of loop", pos);
}

void DiagnosticBag::report_invalid_continue(Position pos)
{
    report("ERROR: 'continue' outside of loop", pos);
}

void DiagnosticBag::report_invalid_return(Position pos)
{
    report("ERROR: 'return' outside of function", pos);
}

void DiagnosticBag::report_redefinition(const std::string& name, Position pos)
{
    std::ostringstream os;
    os << "ERROR: redefinition of variable '" << name << "'";
    report(os.str(), pos);
}

void DiagnosticBag::report_runtime_error(const std::string& message, Position pos)
{
    std::ostringstream os;
    os << "ERROR: " << message;
    report(os.str(), pos);
}
