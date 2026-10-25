#include "object.h"

using namespace Objects;

Object::~Object() {}

std::string Object::to_display_string() const
{
    return to_string();
}

// Return none type as default. This will tell me when an illegal operation has occured.
Object* Object::added_by(Object* other) const
{
    return new None();
}

Object* Object::subtracted_by(Object* other) const
{
    return new None();
}

Object* Object::multiplied_by(Object* other) const
{
    return new None();
}

Object* Object::divided_by(Object* other) const
{
    return new None();
}

Object* Object::modded_by(Object* other) const
{
    return new None();
}

Object* Object::powered_by(Object* other) const
{
    return new None();
}

Object* Object::accessed_by(Object* other) const
{
    return new None();
}

Object* Object::and_with(Object* other) const
{
    return new None();
}

Object* Object::or_with(Object* other) const
{
    return new None();
}

Object* Object::xor_with(Object* other) const
{
    return new None();
}

Object* Object::notted() const
{
    return new None();
}

Object* Object::less_than(Object* other) const
{
    return new None();
}

Object* Object::greater_than(Object* other) const
{
    return new None();
}

Object* Object::equals(Object* other) const
{
    return new None();
}

// These can be derived from the other operations.
Object* Object::not_equals(Object* other) const
{
    Object* eq = equals(other);
    Object* res = eq->notted();
    delete eq;
    return res;
}

Object* Object::less_equals(Object* other) const
{
    Object* gt = greater_than(other);
    Object* res = gt->notted();
    delete gt;
    return res;
}

Object* Object::greater_equals(Object* other) const
{
    Object* lt = less_than(other);
    Object* res = lt->notted();
    delete lt;
    return res;
}
