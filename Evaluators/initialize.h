#pragma once

#include "evaluator.h"
#include <string>
#include <vector>

namespace Evaluators
{
    void initialize();
    void reset();
    void set_script_args(const std::vector<std::string>& args);
    void print_symbols();
    bool run(std::string &script, bool show_tree=false, bool show_return=false, bool is_shell=false);
}
