# CS 4280 Project 4: Code Generation
# Spring 2022
Continued from Project 3: Static Semantics

## Objective
Generate assembly code for an input program.
Target language is [VM ACC assembly language](https://comp.umsl.edu/assembler/interpreter).

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
  - if T is `<<`, repeat D until W >= 0
  - if T is `<-`, repeat D until W < 0
- `Spot number` loads `number` into the accumulator (ACC)
- `Spot identifier` reads in an integer value from the user and stores it in new `identifier`
- `Show number` writes `number` to the monitor
- `Show identifier` writes the value of `identifier` to the monitor
- `Move identifier` loads the value of `identifier` into the accumulator
- `Flip identifier` multiplies the value of `identifier` by -1 and stores the result in `identifier`
- `Here number There` writes `number` to the monitor for `number` times

All data values are 2-byte signed integers.
Operations are assumed to have no overflow.
