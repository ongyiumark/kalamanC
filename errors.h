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
	
	std::string arrowed_string() const;
	void print(std::ostream& os) const;
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

#endif