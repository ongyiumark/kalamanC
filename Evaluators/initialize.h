#pragma once

#include "evaluator.h"
#include <string>

namespace Evaluators
{
    void initialize();
    void reset();
    void print_symbols();
    bool run(std::string &script, bool show_tree=false, bool show_return=false, bool is_shell=false);
}
