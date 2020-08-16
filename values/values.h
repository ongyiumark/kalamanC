#ifndef VALUES_H
#define VALUES_H

#include "../position.h"

#include <iostream>
#include <string>

enum ValueType 
{
	INTEGERTYPE,
	DOUBLETYPE,
	STRINGTYPE,
	LISTTYPE,
	FUNCTIONTYPE,
	NULLTYPE
};

const std::string VALUETYPES[] = 
{
	"INTEGER", "DOUBLE", "STRING", "LIST", "FUNCTION", "NULL"
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

	virtual long long get_int_value() const;
	virtual long double get_double_value() const;

	virtual Value* add(const Value* other) const;
	virtual Value* subtract(const Value* other) const;
	virtual Value* multiply(const Value* other) const;
	virtual Value* divide(const Value* other) const;
	virtual Value* modulo(const Value* other) const;
	virtual Value* power(const Value* other) const;
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
};

class Null : public Value
{
public:
	Null();
	void print(std::ostream& os) const;
};

#endif