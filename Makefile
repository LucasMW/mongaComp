test/comp: src/main.c src/lex.c
	cc -o comp src/main.c src/lex.c
src/lex.c: src/rules.lex
	flex src/rules.lex
	mv lex.yy.c src/lex.c
bin/lexical:
	cc -o bin/lexical src/lex.c -ll
clean: 
	rm src/lex.c
	rm -r bin
	mkdir bin
	rm -r temp
	mkdir temp
src/main.o: 
	cc -o temp/main.o -Wall -02 -c main.c
src/lex.o:
	cc -o temp/lex.o	-Wall -02 -c lex.c
bin/comp: temp/main.o temp/lex.o
	ld -o bin/comp main.o temp/lex.o