#always compiles when using just make
test/comp: src/main.c src/lex.c src/grammar.c
	cc -o comp src/main.c src/lex.c src/grammar.c
bin/lexical: src/main.c src/lex.c
	cc -o bin/lexical src/lexmain.c src/lex.c
testsyntax: comp
	cat test/test1.monga |./comp 
	cat test/test2.monga |./comp
	cat test/test3.monga |./comp 
	cat test/test4.monga |./comp
	cat test/test5.monga |./comp 
#testlexical is unavaible
testlexical: comp
	sh test/script.sh
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
	cc -o temp/main.o -Wall -02 -c main.c
src/lex.o:
	cc -o temp/lex.o	-Wall -02 -c lex.c
bin/comp: temp/main.o temp/lex.o
	ld -o bin/comp main.o temp/lex.o