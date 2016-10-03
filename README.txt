1310844
Lucas Menezes, T2 1715
—————————————————————

Description of monga language:
http://www.inf.puc-rio.br/~roberto/comp/lang.html

First, use must use make to compile:
make

To run the tests for the syntax, use
make testsyntax


To clean builds, use
make clean


make testlexical is currently unavaible

All testlexical exclusive parts are currently unavaible

Requirements:
C compiler
make
flex
bison
bash

rules.lex was developed using flex. 
Although, it was written to also work in lex, it was never tested.

grammar.y was developed using bison.
If bison and yacc differ, bison output should be considered correct for this project


Folder structure:
src/ contains the source code: rules.lex,lex.h and main.c.
lex.c is generated form rules.lex by using flex.
grammar.c & grammar.h are generated using bison

test/ contains test suites. Each folder is a collection of test cases.
For instance, take the simple folder which contains most of the tests
Each test have a name and consist of two files: name.monga and name.answer
name.monga is the input the lexical analyzer receives and name.answer, the expected output.
They are compared textually. If they are equal, the test is OK. 
If they are different, the difference is shown on console

the test script is found at test/script.sh .

temp/ is a folder for between compilation and is not currently used

