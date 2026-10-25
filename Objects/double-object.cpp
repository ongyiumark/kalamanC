#include "object.h"
#include <cmath>
#include <iomanip>
#include <limits>

using namespace Objects;

Double* Double::_free_list = nullptr;

void* Double::operator new(size_t size)
{
    if (_free_list)
    {
        Double* p = _free_list;
        _free_list = *reinterpret_cast<Double**>(p);
        return p;
    }
    return ::operator new(size);
}

void Double::operator delete(void* p) noexcept
{
    if (!p) return;
    *reinterpret_cast<Double**>(p) = _free_list;
    _free_list = static_cast<Double*>(p);
}

Double::Double(long double value) : _value(value) {}

Type Double::type() const
{
    return Type::DOUBLE;
}

long double Double::get_value() const
{
    return _value;
}

// Prints up to the digits of precision of a long double, which can be different for every platform.
std::string Double::to_string() const
{
    std::ostringstream os;
    os << std::setprecision(std::numeric_limits<long double>::digits10) << _value;
    return os.str();
}

Object* Double::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(_value + ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value + ((Double*)other)->get_value());
        default:
            return new None();
    }
}

Object* Double::subtracted_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(_value - ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value - ((Double*)other)->get_value());
        default:
            return new None();
    }
}

Object* Double::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(_value * ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value * ((Double*)other)->get_value());
        default:
            return new None();
    }
}

Object* Double::divided_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return new None();
            return new Double(_value / other_int->get_value());
        }
        case Type::DOUBLE:
        {
            Double* other_double = (Double*)other;
            if (other_double->get_value() == 0) return new None();
            return new Double(_value / other_double->get_value());            
        }
        default:
            return new None();
    }
}

Object* Double::modded_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            if (other_int->get_value() == 0) return new None();
            return new Double(fmodl(_value, other_int->get_value()));
        }
        case Type::DOUBLE:
        {
            Double* other_double = (Double*)other;
            if (other_double->get_value() == 0) return new None();
            return new Double(fmodl(_value, other_double->get_value()));
        }
        default:
            return new None();
    }
}

Object* Double::powered_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Double(powl(_value, ((Integer*)other)->get_value())); 
        case Type::DOUBLE:
            return new Double(powl(_value, ((Double*)other)->get_value()));     
        default:
            return new None();       
    }
}

// Returns the respective booleans.
Object* Double::less_than(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value < ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value < ((Double*)other)->get_value());
        default:
            return new None();
    }
}

Object* Double::greater_than(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value > ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value > ((Double*)other)->get_value());
        default:
            return new None();
    }
}

Object* Double::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Boolean(_value == ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Boolean(_value == ((Double*)other)->get_value());
        default:
            return new Boolean(false);
    }
}

Object* Double::copy()
{
    return new Double(_value);
}
