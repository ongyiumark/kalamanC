# KalamanC
Basically, it's an **interpreted programming language** written in C++. 

The **keywords** are **easily replacable**, so you it can be ported to a different language. 

This is my second attempt at a programming language. You can find my first attempt [here](https://github.com/ongyiumark/kalamansi-old). 
It's essentially the same project, but this is much more optimized.
I still don't really know what I'm doing though.

### Name origins
I just asked a friend and they said 'Kalamansi++'. It was originally 'Kalamansi++', but it felt too long so I shortened it to 'KalamanC'. 

# Table of Contents
- [Motivation](#motivation)
- [Features](#features)
- [Code Example](#code_example)
- [Installation](#installation)
- [Language Tutorial](#tutorial)
- [Future Plans](#plans)
- [Acknowledgements](#acknowledgements)

<a name=motivation></a>
# Motivation
Classes just ended and I wanted to try my hand on a technical project, and I came up with this. I wanted to see if I could write a programming language. 
This is actually my very **first personal project** (my first attempt on this doesn't count because they're basically the same thing), so I'm not sure what I'm doing. 
If you're reading this and you know what I'm doing wrong, I'm open to criticsm.

<a name=features></a>
# Features
All the programming languages I know have english keywords, so I wanted to make my own programming language where I could **easily change the keywords**. This can be done by changing a few values in [this](constants.h) header file. 

Right now, I have a Filipino version of the keywords. Just paste the contents of [constants-filipino.h](constants-filipino.h) into the [constants.h](constants.h) file.

As for the syntax, the grammar of the language is summarized in [this](grammar.txt) text file, but it's basically **C++ syntax with python lists and functions**. 

I wanted to make an original language, but I'm not that great at language design. I picked this combination of syntax because I didn't like how whitespace mattered in python, and functions with definite return types in C++ felt too restrictive. I'm open to ideas though.

Also, I implemented builtin matrix operations, so KalamanC can be used to prototype or solve constant space DP problems or linear reccurences. Refer to [this](https://www.youtube.com/watch?v=eMXNWcbw75E) video for more information. You can see how it works in the [tutorial](#tutorial) below.

### Benchmark
The following script runs in **0.490 seconds** while an equivalent script wrtten in python runs in **0.125 seconds**.

```
int i = 0;
while(i < 1000000) i = i + 1;
```

That's about **4 times slower than python**, which is actually pretty decent. My first attempt was over 100 times slower.

<a name=code_example></a>
# Code Example

### In Filipino

```
aka make_list()
{
  lista a = [];
  para (bilang i = 0; i < 10; i = i + 1)
  {
    kung (i == 4) tuloy;
    a = a + i;
  }
  ibalik a;
}

lathala(make_list());    // This will print [0,1,2,3,5,6,7,8,9]
```

### In English

```
def make_list()
{
  list a = [];
  for (int i = 0; i < 10; i = i + 1)
  {
    if (i == 4) continue;
    a = a + i;
  }
  return a;
}

print(make_list());    // This will print [0,1,2,3,5,6,7,8,9]
```

I'll be going over the syntax in more detail below, but if you're familiar C++ and python, you can probably already guess from the example above.

<a name=installation></a>
# Installation

### Clone
Clone this by to your local machine by running

`$ git clone https://github.com/ongyiumark/kalamansi.git`

### Compile
I included a makefile so all you need to do is run 

`$ make`

Assuming you have a C++ compiler, you should now have a `kalman` file which you can run.

### Run
You can run the file without any arguments to run a shell.

`$ ./kalman`

You can also run it with a file containing a **KalamanC script** to run that script.

`$ ./kalman sample.kal`

I chose a `.kal` file extension, but really it can be anything as long as it contains the text of the script.

<a name=tutorial></a>
# Language Tutorial
### Data Types
KalamanC currently has 6 data type:
- bool (boolean)
- bilang (integer) 
- doble (double)
- mgawords (string)
- lista (list)
- func

KalamanC is staticly typed like C++, so delaring and initializing variables have the same syntax as C++.
```
bool b = totoo;               // Declares a boolean 'b', sets to true;
bilang x;                     // Declares an integer 'x', initializes to 0
x = 4;                        // Sets 'x' to 4
doble y = 3.14;               // Declares a double 'y', sets to 3.14
mgawords s = "Hello world!";  // Declares a string 's',  sets to "Hello world!"
lista l = [x, s, y];          // Declares a list 'l', sets to [4, "Hello world", 3.14]
```
Make sure to end your statments with a semicolon. 
This is because I designed KalamanC to completely ignore whitespace (no more python indenting errors).

I'll discuss 'func' later when we get to functions, but basically, it can be used to store functions.
It works the same as the data types above.

If you don't initialize the variables, they have a default value (except for 'func'). 
Integers and doubles initialize to 0, and string and lists initialize to an empty string and an empty list respectively.

Note that you cannot set an integer variable to a different data type. The following will incur a runtime error:

```
bilang x = "Hi";
```

### Operators
We have the following operators: 
- \+, \-, \*, \/, \% (standard arithmetic operators)
- \^  (power operator)
- \<, \>, \<=, \>=, \!=, == (standard comparison operators)
- hindi, ! (standard NOT operator)
- at, && (standard AND operator)
- o, || (standard OR operator)
- xo (standard XOR operator)

##### Arithmetic operations
```
(2+1)^2*2;    // Evaluates to 18
1+1 > 32;     // Evaluates to false
12 > 6 > 3;   // The incurs an error.

13 at 0;      // This incurs an error because logical operators can only be used on booleans
```

The third example is an error because it first evaluates (12 > 6) as true, and evaluates (true > 3), which is an illegal operation.

##### String operations
String concatination and multiplication works just like python.
```
"Yes" + "no";       // Evaluates to "Yesno"
"yes"*3;            // Evaluates to "yesyesyes"         
```

##### Matrix operations
If a list is a 2D array of integers, then it is a valid matrix. These only apply for valid matrices.
```
[[1,1]]*[[2],[1]];    // Evaluates to [[3]]

[[1,1],[1,0]]^10;     // Evaluates to [[89, 55], [55, 34]]

```

The second example is actually the matrix representation of the fibonacci sequence. Notice that the last 10th fibonacci number appears in the result.

### Conditionals
Conditionals in kalamanC work the same in C++. The keywords are:
- kung (standard IF)
- ehkung (standard ELIF)
- kundi (standard ELSE)

```
bilang x = 5;
kung (x > 5) lathala("x is greater than 5"); // This is a builtin function that prints
ehkung (x > 3) lathala("x is greater than 3");
kundi lathala("I don't care");
```

Remember that whitespace don't matter in this language so this is completely legal too. 
You can even choose to write everything in one line, but that's ugly.

```
kung (x > 5)
  lathala(x);
```

However, if you want to have multiple statements, you must enclose then in block.

```
kung (x > 5)
{
    int y = x;
    lathala(y);
}
```

If for some reason, you want to do nothing, you can put a semicolon. This is equivalent to `pass` in python.

```
kung (x > 5);
ehkung (x > 3) lathala("Yes");
```

### Loops
Loops in kalamanC work the same in C++. The keywords are:
- habang (standard WHILE)
- para (C++ FOR)
- tuloy (standard CONTINUE)
- hinto (standard BREAK)

```
bilang i = 0;
habang(i < 10)
{
    lathala(i);
    i = i + 1;
}

para (bilang i = 0; i < 10; i = i + 1) lathala(i);
```

These two loops do the same thing. The body of the loops have the same syntax as the body of the conditionals.


### Functions
#### Builtin Functions
##### lathala() 
Arguments: Any literal or variable <br>
Returns: NullType <br>
Prints the value to the screen 

##### input()
Arguments: None <br>
Returns: StringType <br>
Reads input from user.

##### to_int()
Arguments: 
- string, integer, double, or boolean <br>
Returns IntegerType
Coverts the given value to an integer

##### to_double() (not yet implemented)
##### to_string() (not yet implemented)
##### type()  (not yet implemented)

#### User-defined Functions
This works very similar to python functions. The keywords are:
- aka (python DEF)
- ibalik (standard RETURN)

```
aka gcd(a,b)
  kung (b == 0) ibalik a;
  kundi ibalik gcd(b, a%b);
  
lathala(gcd(10,25));      // This prints 5
```

This is an implentation of a gcd function in KalamanC. 
Notice that I didn't need to enclose it in curly braces because the conditional is a single statement.

You can also store functions into a variable

```
func f = gcd;
lathala(f(10,25));      // This prints 5
```

### Indexing

You can access elements of a list or a string with square brackets. They work similar to python where you can provide a negative index.

```
lista a = ["Yes", "No", 3, [3,1,3.5]];
lathala(a[0][-1]);      // Prints "s"
lathala(a[-1][-1]);     // Prints 3.5
```

Lists and strings are immutable, so you can't reassign their values like this.

```
a[0] = 3;   // This will incur a runtime error
```

### Comments
Comments work the same as C++.

```
// This is a single line comment
/* This
is a multi
line comment */
```

<a name=plans></a>
# Future Plans
- Add more built in functions
- Add a mutable list/collection
- Improve error messages (it's pretty hard to debug if you have the wrong syntax)

<a name=acknowledgements></a>
# Acknowledgments

This was largely based on CodePulse's [Make YOUR OWN Programming Language](https://www.youtube.com/playlist?list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD) Series on YouTube.
The series was written in python so I had to make a couple of adjustments. I also implemented my own syntax.
