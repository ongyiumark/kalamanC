#include "errors.h"
#include "constants.h"

////////////////////////////////////
// ERROR BASE CLASS
////////////////////////////////////
Error::Error(std::string n, std::string dets, Position s, Position e)
	: name(n), details(dets), start(s), end(e) {}

// Generates a string that points at the error
std::string Error::arrowed_string() const
{
	std::string line, arrows;
	std::string script = *start.get_filetext();
	
	int i = start.get_ln_start();
	char c = script[i];
	while((i-end.get_ln_start() < end.get_col() || c != '\n') && i < script.size())
	{
		c = script[i];
		if (c == '\n' || c == '\t') 
		{	
			i++;
			continue;
		}

		line += c;
		if (i-start.get_ln_start() >= start.get_col() 
			&& i-end.get_ln_start() <= end.get_col())
			arrows += "^";
		else arrows += " ";

		i++;
	}

	return line+"\n"+arrows;
}

void Error::print(std::ostream& os) const 
{
	os << name << " : " << details << "\n\n";
	os << arrowed_string();
}

std::ostream& operator<<(std::ostream& os, const Error &err)
{
	err.print(os);
	return os;
}

Position Error::get_start() const
{
	return start;
}

Position Error::get_end() const
{
	return end;
}

std::string Error::get_name() const
{
	return name;
}

std::string Error::get_details() const
{
	return details;
}

////////////////////////////////////
// ILLEGAL CHARACTER CLASS
////////////////////////////////////
IllegalCharacter::IllegalCharacter(std::string dets, Position s, Position e)
	: Error("Illegal Character", dets, s, e) {}

////////////////////////////////////
// EXPECTED CHARACTER CLASS
////////////////////////////////////
ExpectedCharacter::ExpectedCharacter(std::string dets, Position s, Position e)
	: Error("Expected Character", dets, s, e) {}

////////////////////////////////////
// ILLEGAL SYNTAX CLASS
////////////////////////////////////
IllegalSyntax::IllegalSyntax(std::string dets, Position s, Position e)
	: Error("Illegal Syntax", dets, s, e) {}


////////////////////////////////////
// RUNTIME ERROR CLASS
////////////////////////////////////
RuntimeError::RuntimeError(std::string dets, Position s, Position e, Context* ctx)
	: Error("Runtime Error", dets, s, e), context(ctx) {}

std::string RuntimeError::generate_traceback() const
{
	std::string traceback;
	Position pos = get_start();
	Context* ctx = context;

	while(ctx)
	{
		std::string curr = "   File " + pos.get_filename();
		curr += ", line " + std::to_string(pos.get_ln()+1);
		curr += ", col " + std::to_string(pos.get_col());
		curr += ", in " + ctx->get_name() + "\n";
		traceback = curr + traceback;

		pos = ctx->get_parent_pos();
		ctx = ctx->get_parent();
	}
	return "Traceback (most recent call last):\n"+traceback;
}

void RuntimeError::print(std::ostream& os) const
{
	os << get_name() << " : " << get_details() << "\n";
	os << generate_traceback() << "\n";
	os << arrowed_string();
}