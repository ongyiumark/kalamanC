#include "object.h"

#include <iostream>
using namespace Objects;

long long List::matrix_mod = 1'000'000'007;

List::List(std::vector<Object*>& values) : _values(values) {}

List::~List()
{
    for (auto &o : _values)
        delete o;
}

Type List::type() const
{
    return Type::LIST;
}

int List::get_size() const
{
    return _values.size();
}

Object* List::get_value(int i) const
{
    return _values[i];
}

std::vector<Object*> List::get_values() const
{
    return _values;
}

// Comma delimited values, enclose in square brackets.
std::string List::to_string() const
{
    std::ostringstream os;
    bool is_mat = is_matrix(this);
    int n = _values.size();
    os << "[";
    for (int i = 0; i < n; i++)
    {
        os << _values[i]->to_string();
        if (i != n-1) 
            os << (is_mat ? ",\n " : ", ");
    }
    os << "]";
    return os.str();
}

// Checks if a given list is a valid matrix.
// It checks if it's a 2D array of integers.
bool List::is_matrix(const List* list)
{
    int n = list->get_size();
    if (n == 0) return false;
    int m = -1;
    for (int i = 0; i < n; i++)
    {
        if (list->get_value(i)->type() != Type::LIST) 
            return false;
        
        List* child_list = (List*)(list->get_value(i));
        if (m == -1) m = child_list->get_size();
        if (m != child_list->get_size()) return false;
        if (m == 0) return false;
        for (int j = 0; j < m; j++)
            if (child_list->get_value(j)->type() != Type::INTEGER)
                return false;
    }
    return true;
}

// Concatinate the two lists.
// If the other operand is not a list, it appends it to the list.
Object* List::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::LIST:
        {
            std::vector<Object*> result;
            int n = _values.size();
            for (int i = 0; i < n; i++)
                result.push_back(_values[i]->copy());
            
            List* other_list = (List*)other;
            n = other_list->get_size();
            for (int i = 0; i < n; i++)
                result.push_back(other_list->get_value(i)->copy());

            return new List(result);
        }
        default:
        {
            std::vector<Object*> result;
            int n = _values.size();
            for (int i = 0; i < n; i++)
                result.push_back(_values[i]->copy());
            result.push_back(other->copy());
            return new List(result);    
        }
    }
}

// Returns the object at the index.
Object* List::accessed_by(Object* other) const
{
    switch (other->type())
    {
        case Type::INTEGER:
        {
            long long i = ((Integer*)other)->get_value();
            int n = _values.size();
            if (i < 0) i += n;
            if (i < 0 || i >= n) return new None();
            return _values[i]->copy();
        }
        default:
            return new None();
    }
}

// Returns true when all the values in the lists are equal.
Object* List::equals(Object* other) const
{
    switch (other->type())
    {
        case Type::LIST:
        {
            int n = _values.size();
            List* other_list = (List*)other;
            int m = other_list->get_size();
            if (m != n) return new Boolean(false);
            
            bool result = true;
            for (int i = 0; i < n; i++)
            {
                result &= ((Boolean*)(_values[i]->equals(other_list->get_value(i))))->get_value();
            }
            return new Boolean(result);
        }
        default:
            return new Boolean(false);
    }
}

// Matrix multiplication
Object* List::multiplied_by(Object* other) const
{
    switch(other->type())
    {
        case Type::LIST:
        {
            List* other_list = (List*)other;
            if (!is_matrix(this) || !is_matrix(other_list))
                return new None();
            int a = _values.size();
            int b = ((List*)_values[0])->get_size();
            int c = other_list->get_size();
            int d = ((List*)other_list->get_value(0))->get_size();

            if (b != c) return new None();

            std::vector<std::vector<long long>> result(a, std::vector<long long>(d));
            for (int i = 0; i < a; i++)
            {
                for (int j = 0; j < d; j++)
                {
                    for (int k = 0; k < b; k++)
                    {
                        long long left = ((Integer*)((List*)_values[i])->get_value(k))->get_value();
                        long long right = ((Integer*)((List*)other_list->get_value(k))->get_value(j))->get_value();
                        result[i][j] = (result[i][j] + left*right)%matrix_mod;
                    }
                }

            }

            std::vector<Object*> elems;
            for (int i = 0; i < a; i++)
            {
                std::vector<Object*> children;
                for (int j = 0; j < d; j++)
                    children.push_back(new Integer(result[i][j]));
                
                elems.push_back(new List(children));
            }

            return new List(elems);
        }
        default:
            return new None();
    }
}

// Matrix exponentiation
Object* List::powered_by(Object* other) const
{
    switch(other->type())
    {
        case Type::INTEGER:
        {
            if (!is_matrix(this)) return new None();
            int n = _values.size();
            int m = ((List*)_values[0])->get_size();
            if (n != m) return new None();

            long long e = ((Integer*)other)->get_value();
            if (e < 0) return new None();

            // Build identity matrix
            std::vector<Object*> result_elems;
            for (int i = 0; i < n; i++)
            {
                std::vector<Object*> result_children;
                for (int j = 0; j < n; j++)
                    result_children.push_back(new Integer(j == i));
                result_elems.push_back(new List(result_children));
            }
            List* result = new List(result_elems);
            
            // Copying the original matrix
            std::vector<Object*> base_elems;
            for (int i = 0; i < n; i++)
                base_elems.push_back(_values[i]->copy());

            List* base = new List(base_elems);

            while(e > 0)
            {
                if (e&1) 
                {
                    List* new_result = (List*)(result->multiplied_by(base));
                    delete result;
                    result = new_result;
                }
                List* new_base = ((List*)base->multiplied_by(base));
                delete base;
                base = new_base;
                e >>= 1;
            }
            delete base;
            return result;
        }
        default:
            return new None();
    }
}

Object* List::copy()
{
    std::vector<Object*> new_values;
    for (auto &o : _values)
        new_values.push_back(o->copy());
    return new List(new_values);
}