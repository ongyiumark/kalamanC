#include "diagnostic.h"
#include <sstream>

using namespace Diagnostics;

Diagnostic::Diagnostic(std::string& message, Position pos) : _message(message), _pos(pos) {}

std::string Diagnostic::get_message() const
{
    std::ostringstream os;
    if (_pos.ln != -1) os << "In line " << _pos.ln+1 << ", col " << _pos.col+1 << " ";
    os << _message;

    if (_pos.start == -1) return os.str();
    
    std::string text = DiagnosticBag::script.substr(_pos.start, _pos.end-_pos.start);
    if (!text.empty()) os << "\n\n   " << text << "\n";
    return os.str();
}