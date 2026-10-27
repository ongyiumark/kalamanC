#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <memory>

namespace Syntax
{
    class SyntaxNode;
}

namespace Objects
{
    // Enum class of the data types in KalamanC.
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

    // Refer to object-helpers.cpp.
    std::string type_to_string(Type type);

    // Refer to base-object.cpp.
    class Object
    {
    public:
        virtual ~Object();
        
        virtual Type type() const = 0;
        virtual std::string to_string() const = 0;
        virtual std::string to_display_string() const;

        virtual Object* added_by(Object* other) const;
        virtual Object* subtracted_by(Object* other) const;
        virtual Object* multiplied_by(Object* other) const;
        virtual Object* divided_by(Object* other) const;
        virtual Object* modded_by(Object* other) const;
        virtual Object* powered_by(Object* other) const;
        virtual Object* accessed_by(Object* other) const;
        virtual Object* notted() const;
        virtual Object* less_than(Object* other) const;
        virtual Object* greater_than(Object* other) const;
        virtual Object* equals(Object* other) const;
        virtual Object* less_equals(Object* other) const;
        virtual Object* greater_equals(Object* other) const;
        virtual Object* not_equals(Object* other) const;
        virtual Object* and_with(Object* other) const;
        virtual Object* or_with(Object* other) const;
        virtual Object* xor_with(Object* other) const;
        virtual Object* copy() = 0;
    };

    // Refer to boolean-object.cpp.
    class Boolean final : public Object
    {
    private:
        bool _value;
        static Boolean* _free_list;
    public:
        static bool use_filipino;
        static bool lang_locked;

        Boolean(bool value);
        Type type() const override;
        std::string to_string() const override;
        bool get_value() const;

        Object* notted() const override;
        Object* and_with(Object* other) const override;
        Object* or_with(Object* other) const override;
        Object* xor_with(Object* other) const override;
        Object* equals(Object* other) const override;
        Object* copy() override;

        static void* operator new(size_t size);
        static void operator delete(void* p) noexcept;
    };

    // Refer to integer-object.cpp.
    class Integer final : public Object
    {
    private:
        long long _value;
        static Integer* _free_list;
    public:
        Integer(long long value);
        Type type() const override;
        std::string to_string() const override;
        long long get_value() const;
        void set_value(long long value);

        Object* added_by(Object* other) const override;
        Object* subtracted_by(Object* other) const override;
        Object* multiplied_by(Object* other) const override;
        Object* divided_by(Object* other) const override;
        Object* modded_by(Object* other) const override;
        Object* powered_by(Object* other) const override;
        Object* less_than(Object* other) const override;
        Object* greater_than(Object* other) const override;
        Object* equals(Object* other) const override;
        Object* copy() override;

        static void* operator new(size_t size);
        static void operator delete(void* p) noexcept;
    };

    // Refer to double-object.cpp.
    class Double final : public Object
    {
    private:
        long double _value;
        static Double* _free_list;
    public:
        Double(long double value);
        Type type() const override;
        std::string to_string() const override;
        long double get_value() const;

        Object* added_by(Object* other) const override;
        Object* subtracted_by(Object* other) const override;
        Object* multiplied_by(Object* other) const override;
        Object* divided_by(Object* other) const override;
        Object* modded_by(Object* other) const override;
        Object* powered_by(Object* other) const override;
        Object* less_than(Object* other) const override;
        Object* greater_than(Object* other) const override;
        Object* equals(Object* other) const override;
        Object* copy() override;

        static void* operator new(size_t size);
        static void operator delete(void* p) noexcept;
    };

    // Refer to string-object.cpp.
    class String final : public Object
    {
    private:
        std::string _value;   
    public:
        String(const std::string& value);
        
        Type type() const override;
        std::string to_string() const override;
        std::string to_display_string() const override;
        
        const std::string& get_value() const;
        int get_size() const;

        Object* added_by(Object* other) const override;
        Object* multiplied_by(Object* other) const override;
        Object* accessed_by(Object* other) const override;
        Object* slice(long long start, long long end, bool has_start, bool has_end) const;
        Object* less_than(Object* other) const override;
        Object* greater_than(Object* other) const override;
        Object* equals(Object* other) const override;
        Object* copy() override;
    };

    // Refer to list-object.cpp.
    class List final : public Object
    {
    private:
        std::vector<Object*> _values;   
        static bool is_matrix(const List* list);
    public:
        List(const std::vector<Object*>& values);
        ~List() override;
        
        static long long matrix_mod;
        Type type() const override;
        std::string to_string() const override;

        int get_size() const;
        Object* get_value(int i) const;
        void set_value(int i, Objects::Object* val);
        void push(Objects::Object* val);
        Object* pop(long long i = -1);
        const std::vector<Object*>& get_values() const;

        Object* added_by(Object* other) const override;
        Object* accessed_by(Object* other) const override;
        Object* slice(long long start, long long end, bool has_start, bool has_end) const;
        Object* equals(Object* other) const override;

        Object* multiplied_by(Object* other) const override;
        Object* powered_by(Object* other) const override;
        Object* copy() override;
    };

    // Refer to function-object.cpp.
    class Function final : public Object
    {
    private:
        std::string _name;
        std::vector<std::string> _argument_names;
        std::vector<Type> _argument_types;
        std::shared_ptr<Syntax::SyntaxNode> _body;
        bool _built_in;
        int _min_args;
        int _max_args;
    public:
        Function(const std::string& name, const std::vector<std::string>& argument_names,
                 std::shared_ptr<Syntax::SyntaxNode> body, bool built_in = false,
                 int min_args = -1, int max_args = -1);
        Function(const std::string& name, const std::vector<std::string>& argument_names,
                 const std::vector<Type>& argument_types,
                 std::shared_ptr<Syntax::SyntaxNode> body, bool built_in = false,
                 int min_args = -1, int max_args = -1);
        
        Type type() const override;
        std::string to_string() const override;

        const std::string& get_name() const;

        int get_argument_size() const;
        int get_min_args() const;
        int get_max_args() const;
        const std::string& get_argument_name(int i) const;
        const std::vector<std::string>& get_argument_names() const;
        Type get_argument_type(int i) const;
        const std::vector<Type>& get_argument_types() const;
        Syntax::SyntaxNode* get_body() const;
        std::shared_ptr<Syntax::SyntaxNode> get_body_shared() const;
        bool is_built_in() const;

        Object* equals(Object* other) const override;
        Object* copy() override;
    };

    // Refer to none-object.cpp.
    class None final : public Object
    {
    private:
        static None* _free_list;
    public:
        None();
        Type type() const override;
        std::string to_string() const override;

        Object* equals(Object* other) const override;
        Object* copy() override;

        static void* operator new(size_t size);
        static void operator delete(void* p) noexcept;
    };
}
