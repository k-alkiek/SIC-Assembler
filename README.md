# SIC-Assembler
C++ Implementation of a 2-pass Assembler for instructions sets of SIC/XE hypothetical machine from [Introduction to Systems Programming](https://books.google.com.eg/books/about/System_Software.html?id=4VZDnwEACAAJ&source=kp_cover&redir_esc=y) by Leland L. Beck as the final project for a Systems Programming course.

## Features
Supports all instructions and directives mentioned in the book. This includes instructions and directives for:
  * Arithmetic operations
  * Conditional Logic
  * Different addressing modes (immediate, direct and indirect) in addition to PC and BASE addressing
  * Literals and literals pools
  * Control sections and external definitions and references.

## Assembler
The assembley process is done in 2 passes: 
  1. After pass 1, an intermediate file is output which contains all instructions assigned addresses in addition to a symbol table containing defined labels and their values. 
  2. After pass 2, the final object code is produced in an output file
