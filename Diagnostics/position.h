#pragma once

#include <string>

namespace Diagnostics
{
    struct Position final
    {
        std::string text;
        int position;

        Position(std::string _text, int _position);
    };
}