#include "object.h"
#include <cmath>
#include <limits>

using namespace Objects;

Integer* Integer::_free_list = nullptr;

void* Integer::operator new(size_t size)
{
    if (_free_list)
    {
        Integer* p = _free_list;
        _free_list = *reinterpret_cast<Integer**>(p);
        return p;
    }
    return ::operator new(size);
}

void Integer::operator delete(void* p) noexcept
{
    if (!p) return;
    *reinterpret_cast<Integer**>(p) = _free_list;
    _free_list = static_cast<Integer*>(p);
}

void Integer::drain_free_list()
{
    while (_free_list)
    {
        Integer* next = *reinterpret_cast<Integer**>(_free_list);
        ::operator delete(_free_list);
        _free_list = next;
    }
}

Integer::Integer(long long value) : _value(value) {}

Type Integer::type() const
{
    return Type::INTEGER;
}

long long Integer::get_value() const
{
    return _value;
}

void Integer::set_value(long long value)
{
    _value = value;
}

std::string Integer::to_string() const
{
    return std::to_string(_value);
}

// The result becomes a double when the other operand is a double.
Object* Integer::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value + ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value + ((Double*)other)->get_value());
        default:
            return new None();
    }
}

Object* Integer::subtracted_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value - ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value - ((Double*)other)->get_value());
        default:
            return new None();
    }
}

// Multiplying an integer to a string or list duplicates it that integer number of times.
// I used binary multiplication (derived from binary exponentiation) for strings.
Object* Integer::multiplied_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
            return new Integer(_value * ((Integer*)other)->get_value());
        case Type::DOUBLE:
            return new Double(_value * ((Double*)other)->get_value());
        case Type::STRING:
            return repeat_string(((String*)other)->get_value(), _value);
        case Type::LIST:
        {
            if (_value < 0) return new None();
            List* list = (List*)other;
            int n = list->get_size();
            if (n > 0 && (size_t)_value > std::numeric_limits<size_t>::max() / (size_t)n)
                return new None();
            std::vector<Object*> elems;
            try
            {
                elems.reserve((size_t)_value * n);
                for (long long k = 0; k < _value; k++)
                    for (int i = 0; i < n; i++)
                        elems.push_back(list->get_value(i)->copy());
                return new List(elems);
            }
            catch (const std::bad_alloc&)
            {
                for (auto* o : elems)
                    delete o;
                return new None();
            }
        }
        default:
            return new None();
    }
}

// Returns none when division by zero occurs.
Object* Integer::divided_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            long long other_val = other_int->get_value();
            if (other_val == 0) return new None();
            if (_value == std::numeric_limits<long long>::min() && other_val == -1) return new None();
            return new Integer(_value / other_val);
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

Object* Integer::modded_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            Integer* other_int = (Integer*)other;
            long long other_val = other_int->get_value();
            if (other_val == 0) return new None();
            if (_value == std::numeric_limits<long long>::min() && other_val == -1) return new Integer(0);
            return new Integer(_value % other_val);
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

// I used binary exponentiation here if the exponent is a non-negative integer.
Object* Integer::powered_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            long long b = _value;
            long long e = ((Integer*)other)->get_value();
            if (b == 0 && e < 0) return new None();
            if (e < 0) return new Double(powl(b, e));

            long long ans = 1;
            while (e > 0)
            {
                if (e & 1) ans *= b;
                b *= b;
                e >>= 1;
            }
            return new Integer(ans);
        }
        case Type::DOUBLE:
        {
            long double e = ((Double*)other)->get_value();
            if (_value == 0 && e < 0) return new None();
            return new Double(powl(_value, e));
        }
        default:
            return new None();         
    }
}

// Returns the respective booleans.
Object* Integer::less_than(Object* other) const
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

Object* Integer::greater_than(Object* other) const
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

Object* Integer::equals(Object* other) const
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

Object* Integer::copy()
{
    return new Integer(_value);
}
