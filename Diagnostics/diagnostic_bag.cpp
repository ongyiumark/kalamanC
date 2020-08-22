#include "diagnostic.h"

#include <sstream>
#include <iostream>

using namespace Diagnostics;

DiagnosticBag::DiagnosticBag() {}

std::vector<Diagnostic> DiagnosticBag::_diagnostics = std::vector<Diagnostic>(); 

bool DiagnosticBag::to_continue = false;
bool DiagnosticBag::to_break = false;
Objects::Object* DiagnosticBag::return_value = NULL;

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

void DiagnosticBag::report(Position pos, std::string message)
{
    _diagnostics.push_back(Diagnostic(pos, message));
}

void DiagnosticBag::print()
{
    int n = _diagnostics.size();
    for (int i = 0; i < n; i++)
    {
        //std::cout << "In position " << _diagnostics[i].get_pos().position << ", ";
        std::cout << _diagnostics[i].get_message();
        //std::cout << _diagnostics[i].get_pos().text;
        std::cout << std::endl;
    }
}

void DiagnosticBag::clear()
{
    _diagnostics.clear();
    to_continue = false;
    to_break = false;
}

bool DiagnosticBag::should_return()
{
    return _diagnostics.size() || to_break || to_continue;
}

void DiagnosticBag::report_bad_character(Position pos, char c)
{
    std::ostringstream os;
    os << "ERROR: bad character '" << c << "'";
    report(pos, os.str());
}

void DiagnosticBag::report_invalid_type(Position pos, std::string type)
{
    std::ostringstream os;
    os << "ERROR: '" << pos.text << "' is not a valid " << type;
    report(pos, os.str());
}

void DiagnosticBag::report_expected_character(Position pos, char c)
{
    std::ostringstream os;
    os << "ERROR: expected character '" << c << "'";
    report(pos, os.str());
}

void DiagnosticBag::report_unexpected_token(Position pos, std::string actual, std::string expected)
{
    std::ostringstream os;
    os << "ERROR: unexpected token <" << actual << ">, ";
    os << "expected <" << expected << ">";
    report(pos, os.str());
}

void DiagnosticBag::report_illegal_unary_operation(std::string operation, std::string operand)
{
    std::ostringstream os;
    os << "ERROR: Illegal operation <" << operation << "> for <" << operand << ">"; 
    report(Position("", -1), os.str());
}

void DiagnosticBag::report_illegal_binary_operation(std::string left, std::string operation, std::string right)
{
    std::ostringstream os;
    os << "ERROR: Illegal operation <" << operation << "> between <" << left << ">"; 
    os << " and <" << right << ">";
    report(Position("", -1), os.str());
}

void DiagnosticBag::report_unknown_syntax(std::string syntax)
{
    std::ostringstream os;
    os << "ERROR: Unknown syntax <" << syntax << ">";
    report(Position("", -1), os.str());
}

void DiagnosticBag::report_expected_type(std::string expected, std::string actual)
{
    std::ostringstream os;
    os << "ERROR: expected character <" << expected << ">, ";
    os << "got <" << actual << ">";
    report(Position("", -1), os.str());
}

void DiagnosticBag::report_invalid_assign(std::string actual, std::string expected)
{
    std::ostringstream os;
    os << "ERROR: invalid assignment of <" << actual << "> ";
    os << "to <" << expected << ">";
    report(Position("", -1), os.str());
}

void DiagnosticBag::report_undeclared_variable(std::string identifier)
{
    std::ostringstream os;
    os << "ERROR: undeclared identifier '" << identifier << "'";
    report(Position("", -1), os.str());
}

void DiagnosticBag::report_illegal_arguments(int expected, int actual)
{
    std::ostringstream os;
    os << "ERROR: expected " << expected << ", got " << actual;
    report(Position("", -1), os.str());    
}