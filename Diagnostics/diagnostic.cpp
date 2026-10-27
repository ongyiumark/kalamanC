#include "diagnostic.h"
#include <sstream>
#include <algorithm>

using namespace Diagnostics;

Diagnostic::Diagnostic(const std::string& message, Position pos, const std::vector<std::string>& call_stack)
    : _message(message), _pos(pos), _call_stack(call_stack) {}

Position Diagnostic::get_pos() const
{
    return _pos;
}

std::string Diagnostic::get_message() const
{
    std::ostringstream os;
    if (_pos.ln != -1)
        os << "In line " << _pos.ln + 1 << ", col " << _pos.col + 1 << ": ";
    os << _message;

    if (!_call_stack.empty())
    {
        os << "\n  Traceback (most recent call last):";
        if (_call_stack.size() <= 20)
        {
            for (const auto& frame : _call_stack)
                os << "\n    in " << frame;
        }
        else
        {
            for (size_t i = 0; i < 5; i++)
                os << "\n    in " << _call_stack[i];
            os << "\n    ... [" << (_call_stack.size() - 10) << " frames omitted] ...";
            for (size_t i = _call_stack.size() - 5; i < _call_stack.size(); i++)
                os << "\n    in " << _call_stack[i];
        }
    }

    if (_pos.ln == -1 || DiagnosticBag::script.empty())
        return os.str();

    // Extract the target line from DiagnosticBag::script
    int current_ln = 0;
    size_t line_start = 0;
    const std::string& src = DiagnosticBag::script;
    while (current_ln < _pos.ln && line_start < src.size())
    {
        size_t next_nl = src.find('\n', line_start);
        if (next_nl == std::string::npos)
        {
            line_start = src.size();
            break;
        }
        line_start = next_nl + 1;
        current_ln++;
    }

    if (current_ln == _pos.ln && line_start <= src.size())
    {
        size_t line_end = src.find('\n', line_start);
        if (line_end == std::string::npos)
            line_end = src.size();
        if (line_end > line_start && src[line_end - 1] == '\r')
            line_end--;

        std::string line_text = src.substr(line_start, line_end - line_start);
        if (!line_text.empty())
        {
            std::string ln_str = std::to_string(_pos.ln + 1);
            std::string gutter_pad(ln_str.size(), ' ');
            os << "\n\n  " << ln_str << " | " << line_text << "\n";
            os << "  " << gutter_pad << " | ";

            int col = std::max(0, _pos.col);
            for (int i = 0; i < col && i < (int)line_text.size(); i++)
                os << (line_text[i] == '\t' ? '\t' : ' ');

            int span = std::max(1, _pos.end - _pos.start);
            int max_span = std::max(1, (int)line_text.size() - col);
            span = std::min(span, max_span);
            os << "^";
            for (int i = 1; i < span; i++)
                os << "~";
        }
    }

    return os.str();
}
