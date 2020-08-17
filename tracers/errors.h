#ifndef ERRORS_H
#define ERRORS_H

#include "position.h"
#include "context.h"
#include <string>
#include <iostream>

class Error
{
private:
	std::string name, details;
	Position start, end;
	
public:
	Error(std::string n, std::string dets, Position s, Position e);
	
	virtual std::string arrowed_string() const;
	virtual void print(std::ostream& os) const;

	Position get_start() const;
	Position get_end() const;
	std::string get_name() const;
	std::string get_details() const;
};

std::ostream& operator<<(std::ostream& os, const Error &err);

class IllegalCharacter : public Error
{
public:
	IllegalCharacter(std::string dets, Position s, Position e);
};

class ExpectedCharacter : public Error
{
public:
	ExpectedCharacter(std::string dets, Position s, Position e);
};

class IllegalSyntax : public Error
{
public:
	IllegalSyntax(std::string dets, Position s, Position e);
};

class RuntimeError : public Error
{
private:
	Context* context;
	
public:
	RuntimeError(std::string dets, Position s, Position e, const Context* ctx);
	std::string generate_traceback() const;
	void print(std::ostream& os) const;
};

#endif