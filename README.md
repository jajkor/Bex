# Bex - Boolean Expression Interpreter

Bex is a simple interpreter for a language designed to work with boolean expressions and circuits.

## Building the Project

1. Make sure you have CMake installed
2. Create a build directory: `mkdir build && cd build`
3. Run CMake: `cmake ..`
4. Build the project: `make`

## Running Bex

You can run Bex in two ways:

1. Interactive prompt: `./bex`
2. Run a script: `./bex example.bx`

## Command-line Options

- `-d, --debug`: Enable debug output
- `-h, --help`: Print help information

## Language Features

### Basic Types

- **Bit**: A single boolean value (`true` or `false`)
- **Bit Vector**: A sequence of boolean values

### Boolean Operations

- `not`: Negation
- `and`: Logical AND (can have multiple operands)
- `or`: Logical OR (can have multiple operands)
- `xor`: Exclusive OR
- `xnor`: Exclusive NOR
- `nand`: NAND gate
- `nor`: NOR gate

### Statements

- Bit declaration: `(bit name value)`
- Bit vector declaration: `(bit_vector name values...)`
- Circuit definition: `(circuit name (params...) body...)`
- Print statement: `(print expression)`
- Return statement: `(return expression)`

### Example Code

```lisp
;; Define bits
(bit a true)
(bit b false)
(bit c (not a))

;; Print values
(print a)
(print b)
(print c)

;; Operations
(print (and a b))
(print (or a b))
(print (xor a b))

;; Bit vectors
(bit_vector vec1 1 0 1 0)
(print vec1)

;; Circuit definition (half adder)
(circuit HALF_ADDER (a b)
  (bit s (xor a b))
  (bit c (and a b))
  (return s)
)

;; Circuit usage
(print (HALF_ADDER true false))
```

### Language Grammar

```
<program>        ::= <statement>*
<statement>      ::= <definition> | <expression>
<definition>     ::= <circuit-def> | <bit-def> | <bit-vector-def>
<circuit-def>    ::= '(' 'circuit' IDENTIFIER <parameters> <expression>+ ')'
<parameters>     ::= '(' IDENTIFIER* ')'
<bit-def>        ::= '(' 'bit' IDENTIFIER <expression> ')'
<bit-vector-def> ::= '(' 'bit_vector' IDENTIFIER <expression>+ ')'
<expression>     ::= <literal> | <variable-ref> | <operation> | <call> | '(' <expression> ')'
<operation>      ::= <unary-op> | <binary-op> | <multi-op>
<unary-op>       ::= '(' 'not' <expression> ')'
<binary-op>      ::= '(' <binary-operator> <expression> <expression> ')'
<multi-op>       ::= '(' <multi-operator> <expression>+ ')'
<binary-operator> ::= 'xor' | 'xnor' | 'nand' | 'nor'
<multi-operator>  ::= 'and' | 'or'
<call>           ::= '(' IDENTIFIER <expression>* ')'
<variable-ref>   ::= IDENTIFIER
<literal>        ::= 'true' | 'false'
<print-stmt>     ::= '(' 'print' <expression> ')'
<return-stmt>    ::= '(' 'return' <expression> ')'
```
