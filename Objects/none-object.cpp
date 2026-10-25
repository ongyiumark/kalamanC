#include "object.h"

using namespace Objects;

None* None::_free_list = nullptr;

void* None::operator new(size_t size)
{
    if (_free_list)
    {
        None* p = _free_list;
        _free_list = *reinterpret_cast<None**>(p);
        return p;
    }
    return ::operator new(size);
}

void None::operator delete(void* p) noexcept
{
    if (!p) return;
    *reinterpret_cast<None**>(p) = _free_list;
    _free_list = static_cast<None*>(p);
}

None::None() {}

Type None::type() const
{
    return Type::NONE;
}

// Prints '<none>'
std::string None::to_string() const
{
    return "<none>";
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

Object* None::copy()
{
    return new None();
}
