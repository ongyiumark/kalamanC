#include "object.h"

using namespace Objects;

Object::~Object() {}


// I'm not sure if this is more efficient, but I'll just return the same none object for everything.
Object* Object::none_result = new None();

// Return none type as default. This will tell me when an illegal operation has occured.
Object* Object::added_by(Object* other) const
{
    return none_result;
}

Object* Object::subtracted_by(Object* other) const
{
    return none_result;
}

Object* Object::multiplied_by(Object* other) const
{
    return none_result;
}

Object* Object::divided_by(Object* other) const
{
    return none_result;
}

Object* Object::modded_by(Object* other) const
{
    return none_result;
}

Object* Object::powered_by(Object* other) const
{
    return none_result;
}

Object* Object::accessed_by(Object* other) const
{
    return none_result;
}

Object* Object::and_with(Object* other) const
{
    return none_result;
}

Object* Object::or_with(Object* other) const
{
    return none_result;
}

Object* Object::xor_with(Object* other) const
{
    return none_result;
}

Object* Object::notted() const
{
    return none_result;
}

Object* Object::less_than(Object* other) const
{
    return none_result;
}

Object* Object::greater_than(Object* other) const
{
    return none_result;
}

Object* Object::equals(Object* other) const
{
    return none_result;
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



