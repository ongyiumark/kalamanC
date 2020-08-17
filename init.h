#ifndef INIT_H
#define INIT_H

#include "interpreter/interpreter.h"

RTResult run(std::string filename, std::string script, bool show_output);
void run_file(std::string filename);
void set_builtins();

#endif