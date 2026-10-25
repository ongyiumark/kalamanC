# KalamanC 2.0
**KalamanC** is a fast, lightweight **interpreted programming language** written in C++17 with **native bilingual support** (both **Filipino** and **English** keywords work out of the box), C++-style blocks, Python-style lists & slicing, and built-in **matrix operations** for competitive programming and dynamic programming.

### Name origins
I just asked a friend and they said 'Kalamansi++'. It was originally 'Kalamansi++', but it felt too long so I shortened it to 'KalamanC'. 

# Table of Contents
- [Motivation](#motivation)
- [Features](#features)
- [Code Example](#code_example)
- [Installation & Usage](#installation)
- [Language Tutorial](#tutorial)
- [Built-in Functions](#builtins)
- [Acknowledgements](#acknowledgements)

<a name=motivation></a>
# Motivation
Classes just ended and I wanted to try my hand on a technical project, and I came up with this. I wanted to see if I could write a programming language.

<a name=features></a>
# Features
- **Bilingual Keywords Out of the Box**: Write scripts in **Filipino** (`aka`, `bilang`, `lista`, `kung`, `para`, `lathala`, ...) or **English** (`def`/`define`, `int`, `list`, `if`, `for`, `print`, ...) without recompiling! You can also customize keywords in [`constants-english.h`](constants-english.h) and [`constants-filipino.h`](constants-filipino.h).
- **C++ Syntax + Python Lists & Slicing**: Whitespace-independent C++ block syntax combined with dynamic lists, negative indexing (`a[-1]`), slicing (`a[1:4]`, `a[:3]`), and `for (x in list)` / `para (x sa lista)` iteration.
- **Built-in Matrix Operations**: 2D integer lists support matrix multiplication (`A * B`) and fast binary matrix exponentiation (`A ^ n`) with configurable modulo via `set_mod(m)`, ideal for solving linear recurrences and constant-space DP problems (see [Errichto's Matrix Exponentiation](https://www.youtube.com/watch?v=eMXNWcbw75E)).
- **Interactive REPL Shell**: Supports multi-line blocks, arrow-key command history, session inspection (`#vars`), AST visualization (`#showtree`), and language mode switching (`#lang`).
- **Rich Error Diagnostics**: Clear error messages with source line previews, `^~~~` caret indicators, and function call stack tracebacks.

### Benchmark
The following 1,000,000-iteration loop runs in **~0.033 seconds** (`i++`) / **~0.085 seconds** (`i = i + 1`), outperforming Python (~0.125 seconds):

```cpp
int i = 0;
while (i < 1000000) i++;
```

<a name=code_example></a>
# Code Example

### In Filipino (`examples/filipino_demo.kal`)

```cpp
aka make_list()
{
  lista a = [];
  para (bilang i = 0; i < 10; i++)
  {
    kung (i == 4) tuloy;
    idagdag(a, i);
  }
  ibalik a;
}

lathala(make_list());    // Prints [0, 1, 2, 3, 5, 6, 7, 8, 9]
```

### In English

```cpp
def make_list()
{
  list a = [];
  for (int i = 0; i < 10; i++)
  {
    if (i == 4) continue;
    push(a, i);
  }
  return a;
}

print(make_list());    // Prints [0, 1, 2, 3, 5, 6, 7, 8, 9]
```

<a name=installation></a>
# Installation & Usage

### Build & Test
```bash
git clone https://github.com/ongyiumark/kalamanC.git
cd kalamanC
make -j4
make test
```

### Run Interactive Shell (REPL)
```bash
./kalman
```

Inside the REPL, you can write single-line or multi-line statements and use shell commands:
- `#help` — show available REPL commands
- `#showtree` — toggle syntax parse tree visualization
- `#showreturn` — toggle printing expression return values
- `#lang [english|filipino|auto]` — set boolean display language (`true`/`false` vs `totoo`/`mali`)
- `#vars` — inspect all defined variables and functions
- `#reset` — reset the session state
- `#cls` / `#clear` — clear the screen
- `#exit` / `#quit` — exit the shell

### Run a Script or Inline Code
```bash
# Run a script file
./kalman examples/filipino_demo.kal
./kalman examples/fibonacci_matrix.kal
./kalman examples/algorithms.kal

# Execute inline code directly
./kalman -e 'print([[1, 1], [1, 0]] ^ 10);'
```

<a name=tutorial></a>
# Language Tutorial

### Data Types
KalamanC has 6 data types (usable with either Filipino or English keywords):

| Type | Filipino Keyword | English Keyword(s) | Default Value |
| :--- | :--- | :--- | :--- |
| Boolean | `bool` | `bool`, `boolean` | `mali` / `false` |
| Integer (64-bit) | `bilang` | `int` | `0` |
| Double | `doble` | `double` | `0` |
| String | `mgawords` | `string` | `""` |
| List | `lista` | `list` | `[]` |
| Function | `function` | `function`, `func` | uninitialized |

```cpp
bool b = totoo;               // Declares a boolean 'b', sets to true
bilang x;                     // Declares an integer 'x', initializes to 0
x = 4;                        // Sets 'x' to 4
doble y = 3.14;               // Declares a double 'y', sets to 3.14
mgawords s = "Hello world!";  // Declares a string 's', sets to "Hello world!"
lista l = [x, s, y];          // Declares a list 'l', sets to [4, 'Hello world!', 3.14]
```

### Operators
- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, `^` (right-associative power)
- **Compound Assignment & Increment**: `+=`, `-=`, `*=`, `/=`, `%=`, `^=`, `++`, `--`
- **Comparison**: `<`, `>`, `<=`, `>=`, `!=`, `==`
- **Logical** (with short-circuit evaluation for AND/OR):
  - NOT: `hindi`, `not`, `!`
  - AND: `at`, `and`, `&&`
  - OR: `o`, `or`, `||`
  - XOR: `xo`, `xor`

##### String operations
Both double-quoted (`"..."`) and single-quoted (`'...'`) strings are supported with escape sequences (`\n`, `\t`, `\"`, `\'`, `\\`).
```cpp
"Yes" + "no";       // Evaluates to 'Yesno'
"yes" * 3;          // Evaluates to 'yesyesyes'
```

##### Matrix operations
Any 2D list of integers is treated as a matrix for `List * List` (matrix multiplication) and `List ^ Integer` (matrix exponentiation):
```cpp
[[1, 1]] * [[2], [1]];    // Evaluates to [[3]]
[[1, 1], [1, 0]] ^ 10;    // Evaluates to [[89, 55], [55, 34]] (10th Fibonacci number!)
```
By default, matrix operations are computed modulo `1,000,000,007` (`1e9 + 7`). You can change the modulus at any time with `set_mod(m)` (or `itakda_mod(m)`), or pass `set_mod(0)` for exact un-modded 64-bit integer arithmetic!

### Indexing, Mutation, and Slicing
Lists and strings support 0-based indexing, negative indices, and `[start:end]` slicing. Lists also support in-place index assignment (`a[i] = val`):
```cpp
lista a = ["Yes", "No", 3, [3, 1, 3.5]];
lathala(a[0][-1]);      // Prints s
lathala(a[-1][-1]);     // Prints 3.5
lathala(a[1:3]);        // Prints ['No', 3]

a[0] = "Maybe";         // Mutates element 0 in-place
a[2] += 10;             // Updates a[2] to 13
```

### Conditionals
- Filipino: `kung` (`if`), `ehkung` (`elif`), `kundi` (`else`)
- English: `if`, `elif`, `else`

```cpp
bilang x = 5;
kung (x > 5) lathala("x is greater than 5");
ehkung (x > 3) lathala("x is greater than 3");
kundi lathala("x is 3 or less");
```

### Loops
- Filipino: `habang` (`while`), `para` (`for`), `sa` (`in`), `tuloy` (`continue`), `hinto` (`break`)
- English: `while`, `for`, `in`, `continue`, `break`

```cpp
// C-style for loop
para (bilang i = 0; i < 5; i++) lathala(i);

// Foreach loop over a list or range
for (int x in range(0, 5)) print(x);
para (bilang x sa [10, 20, 30]) lathala(x);
```

### User-defined Functions
- Filipino: `aka` (`def` / `define`), `ibalik` (`return`)
- English: `def` (or `define`), `return`

```cpp
aka gcd(a, b)
  kung (b == 0) ibalik a;
  kundi ibalik gcd(b, a % b);

lathala(gcd(10, 25));   // Prints 5

function f = gcd;
lathala(f(10, 25));     // Prints 5
```

<a name=builtins></a>
# Built-in Functions

All built-in functions can be called using either their English or Filipino names:

| English | Filipino | Description |
| :--- | :--- | :--- |
| `print(val1, val2, ...)` | `lathala(...)` | Prints space-separated values followed by a newline |
| `input([prompt])` | `basahin([prompt])` | Reads a line of input from the user as a `string` |
| `size(val)` / `len(val)` | `sukat(val)` | Returns the length of a `list` or `string` |
| `push(list, val)` / `append(list, val)` | `idagdag(list, val)` | Appends `val` to `list` in-place |
| `pop(list, [index])` | `alisin(list, [index])` | Removes and returns the element at `index` (default `-1`) |
| `set(list, index, val)` | `palitan(list, index, val)` | Returns a copy of `list` with `list[index]` set to `val` |
| `range([start], stop, [step])` | `saklaw(...)` | Generates a `list` of integers |
| `sort(list)` | `ayusin(list)` | Returns a sorted copy of `list` |
| `reverse(val)` | `baliktadin(val)` | Returns a reversed copy of a `list` or `string` |
| `split(str, [delim])` | `hatiin(str, [delim])` | Splits `str` by `delim` (default `" "`) into a `list` |
| `join(list, [delim])` | `pagdugtungin(list, [delim])` | Joins elements of `list` into a `string` |
| `type(val)` | `uri(val)` | Returns the type name of `val` as a `string` |
| `to_bool(val)` | `gawing_bool(val)` | Converts `val` to `bool` |
| `to_int(val)` | `gawing_bilang(val)` | Converts `val` to `int` |
| `to_double(val)` | `gawing_doble(val)` | Converts `val` to `double` |
| `to_string(val)` | `gawing_mgawords(val)` | Converts `val` to `string` |
| `abs(x)` | `ganap(x)` | Absolute value of an `int` or `double` |
| `min(a, b, ...)` / `min(list)` | `pinakamaliit(...)` | Minimum value |
| `max(a, b, ...)` / `max(list)` | `pinakamalaki(...)` | Maximum value |
| `sqrt(x)` | `ugat(x)` | Square root of `x` as a `double` |
| `pow_mod(base, exp, mod)` | `pow_mod(base, exp, mod)` | Computes `(base ^ exp) % mod` |
| `set_mod(mod)` | `itakda_mod(mod)` | Sets matrix modulus (`0` disables modulo) |
| `read_file(path)` | `basahin_file(path)` | Reads a file's contents as a `string` |
| `write_file(path, content)` | `isulat_file(path, content)` | Writes `content` to `path` |
| `clock()` | `oras()` | Returns elapsed seconds since program start (`double`) |
| `exit([code])` | `lumabas([code])` | Exits the interpreter with status `code` |

<a name=acknowledgements></a>
# Acknowledgments

This project was inspired by CodePulse's [Make YOUR OWN Programming Language](https://www.youtube.com/playlist?list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD) and Immo Landwerth's [Building a Compiler](https://www.youtube.com/playlist?list=PLRAdsfhKI4OWNOSfS7EUu5GRAVmze1t2y).
