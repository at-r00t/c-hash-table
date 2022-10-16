#ifndef STDLIB_H
#include <stdlib.h>
#endif

typedef struct{
	char** array;
	size_t size;
	size_t used;
}Dyarr;

int hash(char* str, int offset)
{
    unsigned long hash = 0;
    int c;

    while (c = *str++)
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash % offset;
}

void init(Dyarr* dyarr)
{
	dyarr->array = malloc(sizeof(char*));
	dyarr->size = 1;
	dyarr->used = 0;
}	

Dyarr* find(Dyarr* table, int table_len, char* word, int autoprint)
{
	int index = hash(word, table_len);
	for(int i=0; i<table[index].used; i++){
		if( strcmp(table[index].array[i], word) == 0){
			if(autoprint)printf("[+] word '%s' is at table[%d]\n", table[index].array[i], index+1);
			return &table[index];
		}
	}
	if(autoprint)printf("[-] word '%s' not found in table ..\n", word);
	return NULL;
}

void append(Dyarr* dyarr, char* word)
{
	if(dyarr->used == dyarr->size){
		dyarr->size++;
		dyarr->array = realloc(dyarr->array, sizeof(char*) * dyarr->size);
	}
	dyarr->array[dyarr->used] = malloc(strlen(word) + 1);
	strcpy(dyarr->array[dyarr->used], word);
	dyarr->used++;
}

void shift_arr(Dyarr* arr, int index)
{
	int i;
	for(i=index+1; i<arr->used; i++){
		void* newloc = realloc(arr->array[i-1], strlen(arr->array[i]));
		if(newloc == NULL)
		{
			while(newloc == NULL)
				{
					puts("not enough sapace ..\nretrying [/]");		
					newloc = realloc(arr->array[i-1], strlen(arr->array[i]) + 1);
				}
		}		
		arr->array[i-1] = newloc;
		strcpy(arr->array[i-1], arr->array[i]);
	}
	--arr->size;
	--arr->used;
}

int del(Dyarr* table, int table_len, char* word)
{
	Dyarr* arr = find(table, table_len, word, 0);
	if(arr == NULL){
		return 0;
	}
	for(int i=0; i < arr->used; i++){
		if(strcmp(arr->array[i], word) == 0){
			shift_arr(arr, i);
			return 1;
		}
	}
}

Dyarr tok_list(char* str, char* delm)
{

	char cpstr[strlen(str)];
	strcpy(cpstr, str);
	char* tok = strtok(cpstr, delm);

	Dyarr arr;
	init(&arr);
	append(&arr, tok);
	while((tok = strtok(NULL, delm)) != NULL){
		append(&arr, tok);
	}
	return arr;
}

void show_table(Dyarr* table, int offset)
{
	puts("-------------");
	for (int x=0; x<offset; x++){
		printf("%-10d[ ", x+1);
		for(int y=0; y<table[x].used; y++){
			printf("%s ,", table[x].array[y]);
		}
		char back = '\b';
		if(table[x].used != table[x].size) back = '\0';
		printf("%c]\n-------------\n", back);
	}
}

int logic(Dyarr* table ,int table_len, char* word)
{
	Dyarr tokens = tok_list(word, " ");
	Dyarr* arr = find(table, table_len, word, 0);
	if(arr != NULL){
		puts("allready exists ..");
		return 1;
	}
	
	if(strcmp(word, "$print") == 0){
		show_table(table, table_len);
		return 1;
	}

	else if(tokens.used > 1){
		if(strcmp(tokens.array[0], "$find") == 0){
			find(table, table_len, tokens.array[1], 1);
		}
		else if(strcmp(tokens.array[0], "$del") == 0){
			switch(del(table, table_len, tokens.array[1])){
				case 0: puts("[-] not exists .."); return 1;
				case 1: puts("[+] deleted .."); return 1;
			}
		}
		return 1;
	}
	return 0;
}




