CXX      ?= g++
CXXFLAGS ?= -O3 -Wall -Wextra -Wno-unused-parameter -std=c++17
LDFLAGS  ?=

UNAME_S := $(shell uname -s 2>/dev/null)
ifeq ($(UNAME_S),Darwin)
    LDFLAGS += -ledit
else ifeq ($(UNAME_S),Linux)
    ifneq ($(wildcard /usr/include/readline/readline.h),)
        LDFLAGS += -lreadline
    else ifneq ($(wildcard /usr/include/editline/readline.h),)
        LDFLAGS += -ledit
    endif
endif

BUILD_DIR := build
TARGET    := kalman

SRCS := \
	program.cpp \
	Objects/base-object.cpp \
	Objects/boolean-object.cpp \
	Objects/integer-object.cpp \
	Objects/double-object.cpp \
	Objects/string-object.cpp \
	Objects/list-object.cpp \
	Objects/function-object.cpp \
	Objects/none-object.cpp \
	Objects/object-helpers.cpp \
	Contexts/context.cpp \
	Contexts/symbol-table.cpp \
	Diagnostics/diagnostic.cpp \
	Diagnostics/diagnostic-bag.cpp \
	Diagnostics/position.cpp \
	Syntax/lexer.cpp \
	Syntax/parser.cpp \
	Syntax/syntax-facts.cpp \
	Syntax/syntax-helpers.cpp \
	Syntax/syntax-node.cpp \
	Syntax/syntax-token.cpp \
	Syntax/Expressions/binary-syntax.cpp \
	Syntax/Expressions/break-syntax.cpp \
	Syntax/Expressions/continue-syntax.cpp \
	Syntax/Expressions/for-syntax.cpp \
	Syntax/Expressions/func-call-syntax.cpp \
	Syntax/Expressions/func-define-syntax.cpp \
	Syntax/Expressions/if-syntax.cpp \
	Syntax/Expressions/index-syntax.cpp \
	Syntax/Expressions/literal-syntax.cpp \
	Syntax/Expressions/none-syntax.cpp \
	Syntax/Expressions/return-syntax.cpp \
	Syntax/Expressions/sequence-syntax.cpp \
	Syntax/Expressions/unary-syntax.cpp \
	Syntax/Expressions/var-access-syntax.cpp \
	Syntax/Expressions/var-assign-syntax.cpp \
	Syntax/Expressions/var-declare-syntax.cpp \
	Syntax/Expressions/while-syntax.cpp \
	Evaluators/evaluator.cpp \
	Evaluators/builtin-functions.cpp \
	Evaluators/initialize.cpp

OBJS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

test: $(TARGET)
	@./tests/run_tests.sh

clean:
	rm -rf $(BUILD_DIR) *.o $(TARGET)

-include $(DEPS)
