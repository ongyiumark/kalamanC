#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "values.h"
#include "errors.h"

#include <iostream>

class RTResult
{
private:
	Value* value, *return_value;
	Error* error;
	bool to_continue, to_break;
public:
	RTResult();

	Value* register_value(RTResult result);
	void reset();
	bool should_return();
	void success(Value* val);
	void failure(Error* err);
	void success_break();
	void success_continue();
	void success_return(Value* val);

	Value* get_value();
	Value* get_return_value();
	Error* get_error();
	bool get_continue();
	bool get_break();

	void print(std::ostream& os) const;
};

std::ostream& operator<<(std::ostream& os, const RTResult& res);

#endif