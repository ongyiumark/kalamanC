#pragma once

#include "evaluator.h"
#include <string>

namespace Evaluators
{
    void initialize();
    void run(std::string &script, bool show_tree=false, bool show_return=false);
}