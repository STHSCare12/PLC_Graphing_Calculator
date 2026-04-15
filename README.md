# 50.051 Programming Language Concept Graphing Calculator

## Introduction

This project focuses on developing a graphing calculator using the C programming language.

The problem statement we have chosen to address is:
> *How might we design and implement a graphing calculator in the C programming language that can parse, evaluate, and visually plot mathematical expressions?*

To achieve this, the program supports:
- Handling of user input
- Interpreting mathematical syntax
- Evaluating the mathematical input
- Generating graphical output 

---

## System Architecture
The program is divided into the following components:

- **Input Manager** - Handles user input
- **Lexer (Tokenizer)** - Converts inputs into tokens
- **Parser** - Builds the expression structure and Abstract Syntax Tree (AST)
- **Evaluator** - Computes expression results
- **Graph Manager** - Renders graphical output

Each component is responsible for a stage of the computation pipeline.

---

## Setup Instructions

### 1. Clone the repository: 
```bash
git clone https://github.com/STHSCare12/PLC_Graphing_Calculator.git
```

### 2. Compile the program:  
**macOS / Linux**
```bash
make
```

**Windows**
```bash
mingw32-make
```
Note: `make` builds the program using instructions defined in the Makefile.

### 3. Run the program:
The program supports:
- stdin mode (interactive input)
- CSV file mode (batch input)

---
## Input Modes
The program supports two types of input:

### 1. Standard Input (stdin)
Run the program without arguments:

**macOS / Linux**
```bash
./calculator
```

**Windows**
```bash
.\calculator
```
The program will prompt the user to enter expressions manually.

### 2. CSV File Input
Run the program with a `.csv` file as an argument:

**macOS / Linux**
```bash
./calculator eq.csv
```

**Windows**
```bash
.\calculator eq.csv
```
Each line in the CSV file can contain one or more expressions separated by commas.

### CSV Format Example
```text
3*x+2, sin(x), x^2+1
x^4+1
4*x+2, cos(x)
```
- Each row = one line in the CSV file
- Expressions are separated by commas
- Each expression is evaluated independently

---
## Instruction Menu
When you run the program, you will see:
```text
1. Simple Calculator
2. Differentiate
3. Show Graphs
4. Exit Programme
```

---
## Modes Explanation
### Mode 1: Simple Calculator
- Accepts numerical expressions only
- Variables are not allowed
- **Examples:**
  - `2 + 3`
  - `sin(45)`
  - `10 / 2`

### Mode 2: Differentiate
- Accepts expressions with a **single variable**: `x`
- Computes the derivative of the expression
- Only `x` is supported (no other variables)
- **Examples:**
  - `x^2`
  - `sin(x)`
  - `3*x + 5`

### Mode 3: Show Graphs
- Accepts expressions with variable `x`
- Plots the graph of the function
- **Examples:**
  - `x^2`
  - `sin(x)`
  - `x^3 + 2*x`

### Mode 4: Exit Programme
- Terminates the application

---
## Notes
### Supported operators
```text
+
-
*
/
^
```
### Supported functions
```text
sin
cos
tan
exp
log
```