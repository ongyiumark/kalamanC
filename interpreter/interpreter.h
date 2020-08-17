#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../values/values.h"
#include "../tracers/errors.h"

#include <iostream>

class RTResult
{
private:
	Value* value, *return_value;
	Error* error;
	bool to_continue, to_break;
public:
	RTResult();

	Value* register_value(const RTResult& result);
	void reset();
	bool should_return() const;
	void success(Value* val);
	void failure(Error* err);
	void success_break();
	void success_continue();
	void success_return(Value* val);

	Value* get_value() const;
	Value* get_return_value() const;
	Error* get_error() const;
	bool get_continue() const;
	bool get_break() const;

	void print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const RTResult& res);

#endif