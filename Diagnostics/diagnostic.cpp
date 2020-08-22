#include "diagnostic.h"

using namespace Diagnostics;

// Yes, this is basically just a string, but I might add other values in the future. 
// For example, position, text, etc.
Diagnostic::Diagnostic(std::string& message) : _message(message) {}

std::string Diagnostic::get_message() const
{
    return _message;
}