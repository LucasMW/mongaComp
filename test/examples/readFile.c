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
int isNull(void* ptr){
	return ptr == NULL;
}

char* readFileFast(const char* path)
{
	long size;
	int read;
	FILE * input;
	char * fileString;
	input = fopen(path,"rb");
	if(!input)
		return NULL;
	fseek(input, 0, SEEK_END);
	size = ftell(input);
	rewind(input);
	fileString = (char*)malloc(size+1);
	if(!fileString)
	{
		fclose(input);
		return NULL;
	}
	read = fread(fileString,1,size,input);
	if(read != size) 
	{
		fclose(input);
		free(fileString);
		return NULL;
	}
	fileString[size] = '\0';
	fclose(input);
	return fileString;
}