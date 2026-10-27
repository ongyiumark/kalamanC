#include "object.h"
#include "../constants.h"

using namespace Objects;

Boolean* Boolean::_free_list = nullptr;
bool Boolean::use_filipino = false;
bool Boolean::lang_locked = false;

void* Boolean::operator new(size_t size)
{
    if (_free_list)
    {
        Boolean* p = _free_list;
        _free_list = *reinterpret_cast<Boolean**>(p);
        return p;
    }
    return ::operator new(size);
}

void Boolean::operator delete(void* p) noexcept
{
    if (!p) return;
    *reinterpret_cast<Boolean**>(p) = _free_list;
    _free_list = static_cast<Boolean*>(p);
}

void Boolean::drain_free_list()
{
    while (_free_list)
    {
        Boolean* next = *reinterpret_cast<Boolean**>(_free_list);
        ::operator delete(_free_list);
        _free_list = next;
    }
}

Boolean::Boolean(bool value) : _value(value) {}

Type Boolean::type() const
{
    return Type::BOOLEAN;
}

bool Boolean::get_value() const
{
    return _value;
}

// Uses the keyword for boolean values based on active language mode. Refer to constants.h.
std::string Boolean::to_string() const
{
    if (use_filipino)
        return _value ? KT_TRUE_FIL : KT_FALSE_FIL;
    return _value ? KT_TRUE : KT_FALSE;
}

// Boolean operations. These are self explanatory.
Object* Boolean::notted() const
{
    return new Boolean(!_value);
}

Object* Boolean::and_with(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value && ((Boolean*)other)->get_value());
        default:
            return new None();
    }
}

Object* Boolean::or_with(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value || ((Boolean*)other)->get_value());
        default:
            return new None();
    }
}

Object* Boolean::xor_with(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value ^ ((Boolean*)other)->get_value());
        default:
            return new None();
    }
}

Object* Boolean::equals(Object* other) const
{
    switch(other->type())
    {
        case Type::BOOLEAN:
            return new Boolean(_value == ((Boolean*)other)->get_value());
        default:
            return new Boolean(false);
    }
}

Object* Boolean::copy()
{
    return new Boolean(_value);
}
