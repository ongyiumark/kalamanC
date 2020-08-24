#include "object.h"

using namespace Objects;

Object::~Object() {}

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
    return equals(other)->notted();
}

Object* Object::less_equals(Object* other) const
{
    return greater_than(other)->notted();
}

Object* Object::greater_equals(Object* other) const
{
    return less_than(other)->notted();
}



