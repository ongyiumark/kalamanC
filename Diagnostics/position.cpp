#include "position.h"

using Diagnostics::Position;

Position::Position(int _ln, int _col, int _start, int _end)
    : ln(_ln), col(_col), start(_start), end(_end) {}

Position::Position()
    : ln(-1), col(-1), start(-1), end(-1) {}