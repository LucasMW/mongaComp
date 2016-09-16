Use make to compile:
make

Description of monga language:
http://www.inf.puc-rio.br/~roberto/comp/lang.html

To run the tests for the lexical, use
make testlexical

To clean builds, use
make clean

Requirements:
C compiler
make
flex
bash

rules.lex was developed using flex. 
Although, it was written to also work in lex, it was never tested.


Folder structure:
src/ contains the source code: rules.lex,lex.h and main.c.
lex.c is generated form rules.lex by using flex.

test/ contains test suites. Each folder is a collection of test cases.
For instance, take the simple folder which contains most of the tests
Each test have a name and consist of two files: name.monga and name.answer
name.monga is the input the lexical analyzer receives and name.answer, the expected output.
They are compared textually. If they are equal, the test is OK. 
If they are different, the difference is shown on console

the test script is found at test/script.sh .

temp/ is a folder for between compilation and is not currently used

