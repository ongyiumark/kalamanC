# Kalamansi
Basically, it's an **interpreted programming language** written in *C++*. 

The **keywords** are **easily replacable**, so you it can be ported to a different language. 

This is my second attempt at a programming language. You can find my first attempt [here](https://github.com/ongyiumark/kalamansi-old). 
It's essentially the same project, but this is much more optimized.
I still don't really know what I'm doing though.

### Name origins
I just asked a friend and they said *'kalamansi++'*. It was originally *'kalamansi++'*, but it felt too long so I shortened it. 

*'kalamanC'* seemed was too punny.

# Table of Contents
- [Motivation](#motivation)
- [Features](#features)
- [Code Example](#code_example)
- [Installation](#installation)
- [Language Tutorial](#tutorial)
- [Future Plans](#plans)
- [Acknowledgements](#acknowledgements)

<a name=features></a>
# Motivation
Classes just ended and I wanted to try my hand on a technical project, and I came up with this. I wanted to see if I could write a programming language. 
This is actually my very **first personal project** (my first attempt on this doesn't count because they're basically the same thing), so I'm not sure what I'm doing. 
If you're reading this and you know what I'm doing wrong, I'm open to criticsm.

<a name=motivation></a>
# Features
All the programming languages I know have english keywords, so I wanted to make my own programming language where I could **easily change the keywords**. This can be done by changing a few values in [this](constants.h) header file. Right now, I have a Filipino version of the keywords.

As for the syntax, the grammar of the language is summarized in [this](grammar.txt) text file, but it's basically ***C++* syntax with python lists and functions**. 

I wanted to make an original language, but I'm not that great at language design. I picked this combination of syntax because I didn't like how whitespace mattered in *python*, and functions with definite return types in *C++* felt too restrictive. I'm open to ideas though.

### Benchmark
The following script runs in **2.733 seconds** while an equivalent script wrtten in python runs in **0.125 seconds**.

```
int i = 0;
while(i < 1000000) i = i + 1;
```

That's about **20 times slower than python**, but that's a huge impovement over my first attempt which was over 100 times slower.

<a name=code_example></a>
# Code Example

### In Filipino

```
aka make_list()
{
  lista a = [];
  mula (bilang i = 0; i < 10; i = i + 1)
  {
    kung (i == 4) tuloy;
    a = a + i;
  }
  ibalik a;
}

lathala(make_list())    // This will print [0,1,2,3,5,6,7,8,9]
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

print(make_list())    // This will print [0,1,2,3,5,6,7,8,9]
```

I'll be going over the syntax in more detail below, but if you know *C++* and *python*, you can probably guess from the example above.

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

You can also run it with a file containing a **kalamansi script** to run that script.

`$ ./kalman sample.kal`

I chose a `.kal` file extension but really, it can be anything as long as it contains the text of the script.

<a name=tutorial></a>
# Language Tutorial
### Data Types

### Operators

### Variables

### Conditionals

### Loops

### Functions

<a name=plans></a>
# Future Plans
- Add more built in functions
- Add a mutable list/collection
- Improve error messages
- Improve runtime (not sure how to)

<a name=acknowledgements></a>
# Acknowledgments

This was largely based on CodePulse's [Make YOUR OWN Programming Language](https://www.youtube.com/playlist?list=PLZQftyCk7_SdoVexSmwy_tBgs7P0b97yD) Series on YouTube.
The series was written in *python* so I had to make a couple of adjustments. I also implemented my own syntax.
