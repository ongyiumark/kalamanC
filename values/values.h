#ifndef VALUES_H
#define VALUES_H

#include "../tracers/position.h"
#include "../constants.h"

#include <iostream>
#include <string>
#include <vector>

class Node;

enum ValueType 
{
	INTEGERTYPE,
	DOUBLETYPE,
	STRINGTYPE,
	LISTTYPE,
	FUNCTIONTYPE,
	BIFUNCTYPE,
	NULLTYPE
};

const std::string VALUETYPES[] = 
{
	"INTEGER", "DOUBLE", "STRING", "LIST", "FUNCTION", "BUILTIN_FUNCTION", "NULL"
};

class Value
{
private:
	ValueType type;
	Position start, end;
public:
	Value(ValueType t);
	virtual void print(std::ostream& os) const = 0;

	void set_position(Position s, Position e);

	ValueType get_type() const;
	Position get_start() const;
	Position get_end() const;

	virtual Value* copy() const = 0;
	virtual long long get_int_value() const;
	virtual long double get_double_value() const;
	virtual std::vector<Value*> get_list_values() const;
	virtual std::string get_string_value() const;
	virtual Node* get_func_body() const;
	virtual std::vector<std::string> get_func_argnames() const;
	virtual std::string get_func_name() const;
	virtual BuiltInName get_bifunc_name() const;

	virtual bool is_true() const = 0;
	virtual Value* add(const Value* other) const;
	virtual Value* subtract(const Value* other) const;
	virtual Value* multiply(const Value* other) const;
	virtual Value* divide(const Value* other) const;
	virtual Value* modulo(const Value* other) const;
	virtual Value* power(const Value* other) const;

	virtual Value* notted() const;
	virtual Value* and_with(const Value* other) const;
	virtual Value* or_with(const Value* other) const;
	virtual Value* xor_with(const Value* other) const;

	virtual Value* equals(const Value* other) const = 0;
	virtual Value* not_equals(const Value* other) const;
	virtual Value* less_than(const Value* other) const = 0;
	virtual Value* greater_than(const Value* other) const = 0;
	virtual Value* less_equals(const Value* other) const;
	virtual Value* greater_equals(const Value* other) const;

};

std::ostream& operator<<(std::ostream& os, const Value &val);

class Integer : public Value
{
private:
	long long value;
public:
	Integer(long long x);
	void print(std::ostream& os) const;

	long long get_int_value() const;

	Value* add(const Value* other) const;
	Value* subtract(const Value* other) const;
	Value* multiply(const Value* other) const;
	Value* divide(const Value* other) const;
	Value* modulo(const Value* other) const;
	Value* power(const Value* other) const;

	bool is_true() const;

	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};

class Double : public Value
{
private:
	long double value;
public:
	Double(long double x);
	void print(std::ostream& os) const;

	long double get_double_value() const;

	Value* add(const Value* other) const;
	Value* subtract(const Value* other) const;
	Value* multiply(const Value* other) const;
	Value* divide(const Value* other) const;
	Value* power(const Value* other) const;

	bool is_true() const;
	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};

class Null : public Value
{
public:
	Null();
	void print(std::ostream& os) const;

	bool is_true() const;
	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};

class List : public Value
{
private:
	std::vector<Value*> values;
public:
	List(std::vector<Value*> vals);
	void print(std::ostream& os) const;

	std::vector<Value*> get_list_values() const;

	Value* add(const Value* other) const;

	bool is_true() const;
	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};

class String : public Value
{
private:
	std::string value;
public:
	String(std::string  x);
	void print(std::ostream& os) const;

	std::string get_string_value() const;

	Value* add(const Value* other) const;
	Value* multiply(const Value* other) const;

	bool is_true() const;
	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};

class Function : public Value
{
private:
	std::string name;
	Node* body;
	std::vector<std::string> arg_names;
public:
	Function(std::string nm, Node* n, const std::vector<std::string>& args_n);
	void print(std::ostream& os) const;

	std::vector<std::string> get_func_argnames() const;
	Node* get_func_body() const;
	std::string get_func_name() const;

	bool is_true() const;
	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};



class BuiltInFunction : public Value
{
private:
	BuiltInName name;
	std::vector<std::string> arg_names;
public:
	BuiltInFunction(BuiltInName nm, const std::vector<std::string>& args_n);
	void print(std::ostream& os) const;

	std::vector<std::string> get_func_argnames() const;
	BuiltInName get_bifunc_name() const;
	std::string get_func_name() const;

	bool is_true() const;
	Value* less_than(const Value* other) const;
	Value* greater_than(const Value* other) const;
	Value* equals(const Value* other) const;
	Value* copy() const;
};


#endif