#include "values.h"

#include <math.h>

////////////////////////////////////
// LIST CLASS
////////////////////////////////////
List::List(std::vector<Value*> vals) : Value(ValueType::LISTTYPE), values(vals) {}

void List::print(std::ostream& os) const 
{
	os << "[";
	int n = values.size();
	for (int i = 0; i < n; i++)
	{
		os << *values[i];
		if (i != n-1) os << ", ";
	}
	os << "]";
}

std::vector<Value*> List::get_list_values() const
{
	return values;
}

Value* List::add(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::LISTTYPE:
		{
			std::vector<Value*> vals = values;
			for (Value* v : other->get_list_values())
				vals.push_back(v);

			return new List(vals);
		}
		default:
		{
			std::vector<Value*> vals = values;
			vals.push_back(other->copy());
			return new List(vals);
		}
	}
}

bool List::is_true() const
{
	bool truth = true;
	for (Value* v : values)
		truth &= v->is_true();
	return truth;
}

Value* List::less_than(const Value* other) const
{
	switch (other->get_type())
	{
		default:
			return new Null();
	}
}

Value* List::greater_than(const Value* other) const
{
	switch (other->get_type())
	{
		default:
			return new Null();
	}
}

Value* List::equals(const Value* other) const
{
	switch (other->get_type())
	{
		case ValueType::LISTTYPE:
		{
			std::vector<Value*> other_vals = other->get_list_values();
			if (values.size() != other_vals.size())
				return new Integer(0);

			bool truth = true;
			int n = values.size();
			for (int i = 0; i < n; i++)
				truth &= (values[i]->equals(other_vals[i]))->is_true();
			
			return new Integer((int)truth);
		}
		default:
			return new Null();
	}
}

Value* List::copy() const
{
	return new List(values);
}