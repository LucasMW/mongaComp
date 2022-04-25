#include <stdio.h>
#include <stdlib.h>

char * readFile(const char* path){
	int size;
	FILE * input;
	char * fileString;
	char c;
	int i;
	input = fopen(path,"rt");
	if(!input)
		return NULL;
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	rewind(input);
	fileString = (char*)malloc(size+1);
	i = 0;
	while(fscanf(input,"%c",&c) != EOF){
		fileString[i] = c;
		i++;
	}
	fileString[size] = '\0';
	fclose(input);
	return fileString;
}