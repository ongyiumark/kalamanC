#pragma once

#include <string>
#include <vector>
#include <sstream>

namespace Objects
{
    enum class Type
    {
        BOOLEAN,
        INTEGER,
        DOUBLE,
        STRING,
        LIST,
        FUNCTION,
        NONE
    };

    std::string type_to_string(Type type);

    class Object
    {
    public:
        virtual Type type() const = 0;
        virtual std::string to_string() const = 0;
    };

    class Boolean final : public Object
    {
    private:
        bool _value;   
    public:
        Boolean(bool value);
        Type type() const;
        std::string to_string() const;
        bool get_value() const;
    };

    class Integer final : public Object
    {
    private:
        int _value;   
    public:
        Integer(int value);
        Type type() const;
        std::string to_string() const;
        int get_value() const;
    };

    class Double final : public Object
    {
    private:
        double _value;   
    public:
        Double(double value);
        Type type() const;
        std::string to_string() const;
        double get_value() const;
    };

    class String final : public Object
    {
    private:
        std::string _value;   
    public:
        String(std::string value);
        
        Type type() const;
        std::string to_string() const;
        
        std::string get_value() const;
        int get_size() const;
    };

    class List final : public Object
    {
    private:
        std::vector<Object*> _values;   
    public:
        List(std::vector<Object*>& values);
        
        Type type() const;
        std::string to_string() const;

        int get_size() const;
        Object* get_value(int i) const;
        std::vector<Object*> get_values() const;
    };

    class BoundExpression;

    class Function final : public Object
    {
    private:
        std::string _name;
        std::vector<std::string> _argument_names;
        BoundExpression* _body;
    public:
        Function(std::string name, std::vector<std::string>& argument_names, BoundExpression* body);
        
        Type type() const;
        std::string to_string() const;

        std::string get_name() const;

        int get_argument_size() const;
        std::string get_argument_name(int i) const;
        std::vector<std::string> get_argument_names() const;
        BoundExpression* get_body() const;
    };

    class None final : public Object
    {
    public:
        None();
        Type type() const;
        std::string to_string() const;
    };
}





