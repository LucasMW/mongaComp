#always compiles when using just make
test/comp: src/main.c src/lex.c src/grammar.c
	cc -o comp src/main.c src/lex.c src/grammar.c src/tree.c src/lextest.c src/symbolTable.c
bin/lexical: src/main.c src/lex.c
	cc -o bin/lexical src/lexmain.c src/lex.c

testtype: comp
	cat test/test8.monga |./comp 

testtree: comp
	cat test/test1.monga |./comp 
	cat test/test2.monga |./comp
	cat test/test3.monga |./comp 
	cat test/test4.monga |./comp
	cat test/test5.monga |./comp 
	cat test/test6.monga |./comp 
	cat test/test7.monga |./comp 

testsyntax: comp
	sh test/syntax/script.sh
#testlexical is unavaible
tree:
	cc -o src/tree.o -Wall -O2 -c tree.c
testlexical: comp
	sh test/script.sh

testleaks: comp
	@rm -f val.out prog.out
	cat test/leak1.monga | valgrind --track-origins=yes ./comp > prog.out 2> val.out
	cat val.out | grep error
	rm -f val.out prog.out
src/grammar.c: src/grammar.y
	bison -d src/grammar.y
	mv grammar.tab.c src/grammar.c
	mv grammar.tab.h src/grammar.h
src/lex.c: src/rules.lex src/grammar.c
	flex src/rules.lex
	mv lex.yy.c src/lex.c
bin/lexical2:
	cc -o bin/lexical2 src/lex.c -ll
clean: 
	rm -f src/lex.c
	rm -f src/grammar.c
	rm -r bin
	mkdir bin
	rm -r temp
	mkdir temp
	rm -rf comp.dSYM
	rm -f test/*/*.output
src/main.o: 
	cc -o temp/main.o -Wall -O2 -c main.c
src/lex.o:
	cc -o temp/lex.o	-Wall -O2 -c lex.c
bin/comp: temp/main.o temp/lex.o
	ld -o bin/comp main.o temp/lex.o