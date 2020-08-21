#include "diagnostic.h"

using namespace Diagnostics;

Diagnostic::Diagnostic(Position pos, std::string message) : _pos(pos), _message(message) {}

Position Diagnostic::get_pos() const
{
    return _pos;
}

std::string Diagnostic::get_message() const
{
    return _message;
}