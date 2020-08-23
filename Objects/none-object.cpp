#include "object.h"

using namespace Objects;

None::None() {}

Type None::type() const
{
    return Type::NONE;
}

// Prints '<none>'
std::string None::to_string() const
{
    std::ostringstream os;
    os << "<none>";
    return os.str();
}

Object* None::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::NONE:
            return new Boolean(true);
        default:
            return new Boolean(false);
    }
}