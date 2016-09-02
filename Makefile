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