#pragma once

#include <string>

namespace Diagnostics
{
    struct Position
    {
        int ln, col, start, end;

        Position(int _ln, int _col, int _start, int _end);
        Position();
    };
}