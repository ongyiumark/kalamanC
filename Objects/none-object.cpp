#include "object.h"

using namespace Objects;

None::None() {}

Type None::type() const
{
    return Type::NONE;
}

std::string None::to_string() const
{
    std::ostringstream os;
    os << "<none>";
    return os.str();
}