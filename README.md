# CS 4280 Project 4: Code Generation
Spring 2022  
Continued from Project 3: Static Semantics

## Objective
Generate assembly code for an input program.  

Target language is [VM ACC assembly language](https://comp.umsl.edu/assembler/interpreter).

## Expected Output
Input will be either a text file or standard input stream. Valid input will contain a program written in the previously defined high-level language. Output for valid input will be an .asm file containing assembly code generated from the input code. Program will throw error if input contains any syntax or semantics errors.

Example input:
```
Name prog1
Spot prog2
Place
Name id1
Home
Show prog2
```

Output .asm file:
```
LOAD 0
STORE prog1
READ prog2
LOAD 0
STORE id1
WRITE prog2
STOP
prog1 0
prog2 0
id1 0
```

## Grammar Semantics
- Delimiters: `{`, `}`, `.`, `Place`, `Home`
- Operators:
  
  *Operators have no precedence and are left associative.*
  | Symbol | Meaning |
  | ------ | ------- |
  | `<<` | less than |
  | `<-` | greater than or equal to |
  | `/` | decrement |
  | `+` | addition |
  | `%` | division (truncated) |
  | `&` | multiplication |
  
- `Name identifier` allocates memory for `identifier` and initializes its value to zero
- `Assign identifier M` assigns the value of M to `identifier`
- `{ If identifier T W D }` performs D if and only if `identifier T W` is true
- `{ Do Again D T W }` repeats D until `T W 0` is false
  - if T is `<<`, repeat D until `W >= 0`
  - if T is `<-`, repeat D until `W < 0`
- `Spot number` loads `number` into the accumulator (ACC)
- `Spot identifier` reads in an integer value from the user and stores it in new `identifier`
- `Show number` writes `number` to the monitor
- `Show identifier` writes the value of `identifier` to the monitor
- `Move identifier` loads the value of `identifier` into the accumulator
- `Flip identifier` multiplies the value of `identifier` by -1 and stores the result in `identifier`
- `Here number There` writes `number` to the monitor for `number` times

*All data values are represented as 2-byte signed integers.*  
*Operations are assumed to have no overflow.*

## Static Semantics
- Variables are declared using `Name identifier` and `Spot identifier`
- Variables must be declared before they can be used for the first time
- A variable name may only be defined once

## BNF Grammar
\<S> is the starting nonterminal.  
  
\<S> -> Name Identifier Spot Identifier \<R> \<E>  
\<R> -> Place \<A> \<B> Home  
\<E> -> Show Identifier  
\<A> -> Name Identifier  
\<B> -> empty | . \<C> . \<B> | \<D> \<B>  
\<C> -> \<F> | \<G>  
\<D> -> \<H> | \<J> | \<K> | \<L> | \<E> | \<F>  
\<F> -> { If Identifier \<T> \<W> \<D> } | { Do Again \<D> \<T> \<W> }  
\<G> -> Here Number There  
\<T> -> << | <-  
\<V> -> + | % | &  
\<H> -> / \<Z>  
\<J> -> Assign Identifier \<D>  
\<K> -> Spot Number Show Number | Move Identifier Show Identifier  
\<L> -> Flip Identifier  
\<W> -> Number \<V> Number | Number .  
\<Z> -> Identifier | Number  

## Lexical Definitions
- All tokens in grammar are case sensitive
- *Alphabet:* a-z A-Z 0-9 & + / % . { } << <- whitespace
- Any character outside of the alphabet will generate an error
- *Identifiers:* begin with a lowercase letter (a-z) and continue with one or more alphanumerics
- *Keywords:* `Again`, `If`, `Assign`, `Move`, `Show`, `Flip`, `Name`, `Home`, `Do`, `Spot`, `Place`, `Here`, `There`
- *Operators and delimiters:* `&`, `+`, `/`, `%`, `.`, `{`, `}`, `<<`, `<-`
- *Numbers:* any sequence of decimal digits (0-9), no sign or decimal point
- *Comments:* start with `*` and end with `*`

