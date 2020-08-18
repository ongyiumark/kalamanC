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
const std::string KT_INT 		= "bilang";		// You may change these values
const std::string KT_DOUBLE 	= "doble";
const std::string KT_STRING 	= "mgawords";
const std::string KT_LIST 		= "lista";
const std::string KT_FUNC 		= "func";

const std::string KT_AND	 	= "at";
const std::string KT_OR 		= "o";
const std::string KT_NOT 		= "hindi";
const std::string KT_XOR 		= "xo";

const std::string KT_IF	 		= "kung";
const std::string KT_ELIF	 	= "ehkung";
const std::string KT_ELSE	 	= "kundi";

const std::string KT_FOR		= "para";
const std::string KT_WHILE	 	= "habang";
const std::string KT_BREAK	 	= "hinto";
const std::string KT_CONTINUE 	= "tuloy";

const std::string KT_FUNCDEF 	= "aka";
const std::string KT_RETURN 	= "ibalik";


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
	V_TRUE,
	V_FALSE,
	PRINT,
	INPUTSTR,
	INPUTINT,
	UNDEFINED // Placeholder. Keep this at the last
};

const std::string BUILTINNAMES[] = 
{
	"totoo", "mali",
	"lathala", "input", "intput"
};

const std::set<std::string> RESERVED = 
{
	BUILTINNAMES[BuiltInName::PRINT],
	BUILTINNAMES[BuiltInName::INPUTSTR],
	BUILTINNAMES[BuiltInName::INPUTINT],
};

#endif