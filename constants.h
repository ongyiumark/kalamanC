#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <map>
#include <set>

////////////////////////////////////
// CONSTANTS
////////////////////////////////////
const std::string IGNORE = " \t\n\r"; // Character to ignore
const std::string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const std::string DIGITS = "0123456789";

const std::map<char, char> ESCAPECHARS = {{'n','\n'}, {'t', '\t'}}; //Escape characters in STRINGTYPE
const int ERRORLIMIT = 70; // Limit of the error message

////////////////////////////////////
// KEYWORDS
////////////////////////////////////
const std::string KT_INT 		= "int";		// You may change these values
const std::string KT_DOUBLE 	= "double";
const std::string KT_STRING 	= "str";
const std::string KT_LIST 		= "list";
const std::string KT_FUNC 		= "func";

const std::string KT_AND	 	= "and";
const std::string KT_OR 		= "or";
const std::string KT_NOT 		= "not";
const std::string KT_XOR 		= "xor";

const std::string KT_IF	 		= "if";
const std::string KT_ELIF	 	= "elif";
const std::string KT_ELSE	 	= "else";

const std::string KT_FOR		= "for";
const std::string KT_WHILE	 	= "while";
const std::string KT_BREAK	 	= "break";
const std::string KT_CONTINUE 	= "continue";

const std::string KT_FUNCDEF 	= "def";
const std::string KT_RETURN 	= "return";


const std::set<std::string> KEYWORDS = 		// Add the keywords here if you add keywords
{
	KT_INT, KT_DOUBLE, KT_STRING, KT_LIST, KT_FUNC,
	KT_AND, KT_OR, KT_NOT, KT_XOR,
	KT_IF, KT_ELIF, KT_ELSE,
	KT_FOR, KT_WHILE, KT_BREAK, KT_CONTINUE,
	KT_FUNCDEF, KT_RETURN
};

////////////////////////////////////
// Built In Functions
////////////////////////////////////
enum BuiltInName
{
	PRINT,
	INPUTSTR,
	INPUTINT,
	UNDEFINED // Placeholder. Keep this at the last
};

const std::string BUILTINNAMES[] = 
{
	"print", "input", "intput"
};

#endif