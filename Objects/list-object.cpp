#include "object.h"

#include <iostream>
#include <algorithm>
using namespace Objects;

long long List::matrix_mod = 1'000'000'007;

List::List(const std::vector<Object*>& values) : _values(values) {}

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

void List::set_value(int i, Object* val)
{
    delete _values[i];
    _values[i] = val;
}

void List::push(Object* val)
{
    _values.push_back(val);
}

Object* List::pop(long long i)
{
    long long n = (long long)_values.size();
    if (n == 0) return new None();
    if (i < 0) i += n;
    if (i < 0 || i >= n) return new None();
    Object* removed = _values[(size_t)i];
    _values.erase(_values.begin() + i);
    return removed;
}

const std::vector<Object*>& List::get_values() const
{
    return _values;
}

// Comma delimited values, enclosed in square brackets.
std::string List::to_string() const
{
    std::ostringstream os;
    bool is_mat = is_matrix(this);
    int n = _values.size();
    os << "[";
    for (int i = 0; i < n; i++)
    {
        os << _values[i]->to_string();
        if (i != n - 1) 
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

// Concatenates the two lists.
// If the other operand is not a list, it appends it to the list.
Object* List::added_by(Object* other) const
{
    switch (other->type())
    {
        case Type::LIST:
        {
            List* other_list = (List*)other;
            int n = _values.size();
            int m = other_list->get_size();
            std::vector<Object*> result;
            result.reserve(n + m);
            for (int i = 0; i < n; i++)
                result.push_back(_values[i]->copy());
            for (int i = 0; i < m; i++)
                result.push_back(other_list->get_value(i)->copy());

            return new List(result);
        }
        default:
        {
            int n = _values.size();
            std::vector<Object*> result;
            result.reserve(n + 1);
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
            long long n = (long long)_values.size();
            if (i < 0) i += n;
            if (i < 0 || i >= n) return new None();
            return _values[(size_t)i]->copy();
        }
        default:
            return new None();
    }
}

Object* List::slice(long long start, long long end, bool has_start, bool has_end) const
{
    long long n = (long long)_values.size();
    long long s = has_start ? start : 0;
    long long e = has_end ? end : n;

    if (s < 0) s += n;
    if (e < 0) e += n;

    s = std::max(0LL, std::min(s, n));
    e = std::max(0LL, std::min(e, n));

    std::vector<Object*> result;
    if (s < e)
    {
        result.reserve((size_t)(e - s));
        for (long long i = s; i < e; i++)
            result.push_back(_values[(size_t)i]->copy());
    }
    return new List(result);
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
            
            for (int i = 0; i < n; i++)
            {
                Object* eq = _values[i]->equals(other_list->get_value(i));
                bool is_eq = (eq->type() == Type::BOOLEAN) && ((Boolean*)eq)->get_value();
                delete eq;
                if (!is_eq) return new Boolean(false);
            }
            return new Boolean(true);
        }
        default:
            return new Boolean(false);
    }
}

// Matrix multiplication (or list repetition when multiplied by an integer)
Object* List::multiplied_by(Object* other) const
{
    switch(other->type())
    {
        case Type::INTEGER:
        {
            long long count = ((Integer*)other)->get_value();
            if (count < 0) return new None();
            int n = _values.size();
            std::vector<Object*> elems;
            elems.reserve((size_t)count * n);
            for (long long k = 0; k < count; k++)
                for (int i = 0; i < n; i++)
                    elems.push_back(_values[i]->copy());
            return new List(elems);
        }
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

            // Pre-extract raw integer values for fast cache-friendly multiplication
            std::vector<long long> mat_a((size_t)a * b);
            std::vector<long long> mat_b((size_t)c * d);
            for (int i = 0; i < a; i++)
            {
                List* row = (List*)_values[i];
                for (int k = 0; k < b; k++)
                    mat_a[(size_t)i * b + k] = ((Integer*)row->get_value(k))->get_value();
            }
            for (int k = 0; k < c; k++)
            {
                List* row = (List*)other_list->get_value(k);
                for (int j = 0; j < d; j++)
                    mat_b[(size_t)k * d + j] = ((Integer*)row->get_value(j))->get_value();
            }

            std::vector<long long> result((size_t)a * d, 0);
            if (matrix_mod > 0)
            {
                for (int i = 0; i < a; i++)
                {
                    for (int k = 0; k < b; k++)
                    {
                        long long left = mat_a[(size_t)i * b + k] % matrix_mod;
                        if (left < 0) left += matrix_mod;
                        if (left == 0) continue;
                        for (int j = 0; j < d; j++)
                        {
                            long long right = mat_b[(size_t)k * d + j] % matrix_mod;
                            if (right < 0) right += matrix_mod;
                            long long idx = (long long)i * d + j;
                            long long term = (long long)((static_cast<__int128_t>(left) * right) % matrix_mod);
                            result[(size_t)idx] = (result[(size_t)idx] + term) % matrix_mod;
                        }
                    }
                }
            }
            else
            {
                for (int i = 0; i < a; i++)
                {
                    for (int k = 0; k < b; k++)
                    {
                        long long left = mat_a[(size_t)i * b + k];
                        if (left == 0) continue;
                        for (int j = 0; j < d; j++)
                        {
                            result[(size_t)i * d + j] += left * mat_b[(size_t)k * d + j];
                        }
                    }
                }
            }

            std::vector<Object*> elems;
            elems.reserve(a);
            for (int i = 0; i < a; i++)
            {
                std::vector<Object*> children;
                children.reserve(d);
                for (int j = 0; j < d; j++)
                    children.push_back(new Integer(result[(size_t)i * d + j]));
                
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
            result_elems.reserve(n);
            for (int i = 0; i < n; i++)
            {
                std::vector<Object*> result_children;
                result_children.reserve(n);
                for (int j = 0; j < n; j++)
                    result_children.push_back(new Integer(j == i));
                result_elems.push_back(new List(result_children));
            }
            List* result = new List(result_elems);
            
            // Copying the original matrix
            std::vector<Object*> base_elems;
            base_elems.reserve(n);
            for (int i = 0; i < n; i++)
                base_elems.push_back(_values[i]->copy());

            List* base = new List(base_elems);

            while (e > 0)
            {
                if (e & 1) 
                {
                    List* new_result = (List*)(result->multiplied_by(base));
                    delete result;
                    result = new_result;
                }
                e >>= 1;
                if (e > 0)
                {
                    List* new_base = ((List*)base->multiplied_by(base));
                    delete base;
                    base = new_base;
                }
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
    new_values.reserve(_values.size());
    for (auto &o : _values)
        new_values.push_back(o->copy());
    return new List(new_values);
}
