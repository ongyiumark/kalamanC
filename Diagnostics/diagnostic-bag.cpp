#include "diagnostic.h"

#include <sstream>
#include <iostream>

using namespace Diagnostics;

std::string DiagnosticBag::script = "";

DiagnosticBag::DiagnosticBag() {}

// I made this static so I just have one bag for the entire program.
std::vector<Diagnostic> DiagnosticBag::_diagnostics = std::vector<Diagnostic>(); 

int DiagnosticBag::size()
{
    return _diagnostics.size();
}

Diagnostic DiagnosticBag::diagnostic(int i)
{
    return _diagnostics[i];
}

std::vector<Diagnostic> DiagnosticBag::diagnostics()
{
    return _diagnostics;
}

// Adds an error to the bag.
void DiagnosticBag::report(std::string message, Position pos)
{
    _diagnostics.push_back(Diagnostic(message, pos));
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
}

// Occurs when the lexer encounters an unknown character.
void DiagnosticBag::report_bad_character(char c, Position pos)
{
    std::ostringstream os;
    os << "ERROR: bad character '" << c << "'";
    report(os.str(), pos);
}

// Occurs when parsing to an integer or double fails.
void DiagnosticBag::report_invalid_type(std::string text, std::string type, Position pos)
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
void DiagnosticBag::report_unexpected_token(std::string actual, std::string expected, Position pos)
{
    std::ostringstream os;
    os << "ERROR: unexpected token <" << actual << ">, ";
    os << "expected <" << expected << ">";
    report(os.str(), pos);
}

// Occurs when a unary operation fails.
void DiagnosticBag::report_illegal_unary_operation(std::string operation, std::string operand, Position pos)
{
    std::ostringstream os;
    os << "ERROR: illegal operation <" << operation << "> for <" << operand << ">"; 
    report(os.str(), pos);
}

// Occurs when a binary operation fails.
void DiagnosticBag::report_illegal_binary_operation(std::string left, std::string operation, std::string right, Position pos)
{
    std::ostringstream os;
    os << "ERROR: illegal operation <" << operation << "> between <" << left << ">"; 
    os << " and <" << right << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator encounters an unknown syntax.
// Realistically this should never occur unless I messed up.
void DiagnosticBag::report_unknown_syntax(std::string syntax, Position pos)
{
    std::ostringstream os;
    os << "ERROR: unknown syntax <" << syntax << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator encounters a different type than expected.
void DiagnosticBag::report_unexpected_type(std::string actual, std::string expected, Position pos)
{
    std::ostringstream os;
    os << "ERROR: unexpected type <" << actual << ">, ";
    os << "expected <" << expected << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator tries to assign a value to a variable of a different type.
void DiagnosticBag::report_invalid_assign(std::string actual, std::string expected, Position pos)
{
    std::ostringstream os;
    os << "ERROR: invalid assignment of <" << actual << "> ";
    os << "to <" << expected << ">";
    report(os.str(), pos);
}

// Occurs when the evaluator tries to access an undeclared identifier.
void DiagnosticBag::report_undeclared_identifier(std::string identifier, Position pos)
{
    std::ostringstream os;
    os << "ERROR: undeclared identifier '" << identifier << "'";
    report(os.str(), pos);
}

// Occurs when the wrong number of arguments are given when calling a function.
void DiagnosticBag::report_illegal_arguments(int actual, int expected, std::string name, Position pos)
{
    std::ostringstream os;
    os << "ERROR: provided with " << actual << " argument(s), expected " << expected;
    report(os.str(), pos);    
}

// Occurs when supposedly unreachable code is reached.
// Again, this should never occur unless I messed up.
void DiagnosticBag::report_unreachable_code(std::string info, Position pos)
{
    std::ostringstream os;
    os << "ERROR: reached unreachable code, " << info;
    report(os.str(), pos);    
}

// Occurs when invalid arguments are passed to a builtin function.
void DiagnosticBag::report_invalid_builtin_arguments(std::string name, int i, std::string type, Position pos)
{
    std::ostringstream os;
    os << "ERROR: argument " << i << " of '" << name << "' cannot be <" << type << ">" ;
    report(os.str(), pos);    
}