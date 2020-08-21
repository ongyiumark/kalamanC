#include "diagnostic.h"

#include <sstream>
#include <iostream>

using namespace Diagnostics;

DiagnosticBag::DiagnosticBag() {}

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

void DiagnosticBag::report(Position pos, std::string message)
{
    _diagnostics.push_back(Diagnostic(pos, message));
}

void DiagnosticBag::print()
{
    int n = _diagnostics.size();
    for (int i = 0; i < n; i++)
    {
        std::cout << "In position " << _diagnostics[i].get_pos().position << ", ";
        std::cout << _diagnostics[i].get_message();
        //std::cout << _diagnostics[i].get_pos().text;
        std::cout << std::endl;
    }
}

void DiagnosticBag::clear()
{
    _diagnostics.clear();
}

void DiagnosticBag::report_bad_character(Position pos, char c)
{
    std::ostringstream os;
    os << "ERROR: bad character '" << c << "'";
    report(pos, os.str());
}

void DiagnosticBag::report_invalid_type(Position pos, Objects::Type type)
{
    std::ostringstream os;
    os << "ERROR: '" << pos.text << "' is not a valid " << Objects::type_to_string(type);
    report(pos, os.str());
}

void DiagnosticBag::report_expected_character(Position pos, char c)
{
    std::ostringstream os;
    os << "ERROR: expected character '" << c << "'";
    report(pos, os.str());
}

