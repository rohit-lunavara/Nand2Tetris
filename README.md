# Nand2Tetris

Creating the hardware of a general purpose computer by building in order :

## Done

1. **Logic Gates**

2. **CPU, RAM and other chips**

3. **Assembler for the Hack assembly language**

## Pending

4. Compiler for the Jack high-level language

5. Jack OS

The Assembler targets the instruction set of the Hack machine language (16-bit) to create binaries for execution.

The project contains support for peripherals such as a black and white Screen (256 x 512 pixels) and a Keyboard.

## Completed Projects

01. Creating basic logic gates such as OR, AND, MUX, DEMUX, etc using NAND as a primitive gate.

02. Creating adders and ALU based on gates constructed earlier.

03. Creating memory chips (RAM, PC) using DFF as a primitive flip-flop.

04. Creating assembly programs to understand how the Hack assembly language implements basic programming constructs such 
as working with registers and memory, branching, variables, iteration, pointers, input / output.

05. Creating the computer chip using the CPU and Memory chips constructed earlier.

06. Creating an Assembler program that translates programs written in the Hack assembly language into binary code 
that can execute on the Hack hardware platform built in the previous projects.

## Future Projects

07. Creating support for stack arithmetic and memory access commands for a Virtual Machine.

08. Creating support for branching and function calling commands for a Virtual Machine.

09. Creating high-level language programs to understand how the Jack (Java-like) programming language implements Sys, Memory,
IO, Screen, Keyboard, Math, String and Array libraries for a programming language.

10. Creating the syntax analyzer for the Jack programming language to emit XML for code generation.

11. Creating the complete compiler from the syntax analyzer for the Jack programming language.

12. Creating the Jack OS which implements the libraries to provide access for the hardware of the computer.
