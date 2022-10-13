#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T_LEN 10
#define BUFF_SIZE 4096

typedef struct{
	char** array;
	size_t size;
	size_t used;
}Dyarr;

int hash(char* str){
	int result = 0;
	for (int i=0; i<sizeof(str); i++){
		result += str[i]; 
	}
	return result % T_LEN;
}

void init(Dyarr* dyarr){
	dyarr->array = malloc(sizeof(char*));
	dyarr->size = 1;
	dyarr->used = 0;
}	

void append(Dyarr* dyarr, char* value){
	//make sure ther's enough space
	if(dyarr->used == dyarr->size){
		dyarr->size++;
		dyarr->array = realloc(dyarr->array, sizeof(char*) * dyarr->size);
	}
	dyarr->array[dyarr->used] = malloc(strlen(value));
	strcpy(dyarr->array[dyarr->used], value);
	dyarr->used++;	
}

void find(Dyarr* table, char* word){
	int index = hash(word);
	for(int i=0; i<table[index].used; i++){
		if( strcmp(table[index].array[i], word) == 0){
			printf("[+] word '%s' is at table[%d]\n", table[index].array[i], index+1);
			return;
		}
	}
	printf("[-] word '%s' not found in table ..\n", word);
}
void show_table(Dyarr* table){
	for (int x=0; x<T_LEN; x++){
		printf("%-10d[", x+1);
		for(int y=0; y<table[x].used; y++){
			printf("%s,", table[x].array[y]);
		}
		char back = '\b';
		if(table[x].used != table[x].size) back = '\0';
		printf("%c]\n", back);
	}
}

Dyarr tok_list(char* str, char* delm){

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

int stdout_logic(Dyarr* table ,char* word){
	Dyarr tokens = tok_list(word, " ");
	if(strcmp(word, "$print") == 0){
		show_table(table);
		return 1;
	}

	else if(tokens.used > 1){
		if(strcmp(tokens.array[0], "$find") == 0){
			find(table, tokens.array[1]);
			return 1;
		}
		return 1;
	}
	return 0;
}


int main(void){
	Dyarr table[T_LEN];
	
	for(int i=0; i<T_LEN; i++){
		init(&table[i]);
	}

	while(1){
		char buffer[BUFF_SIZE];
		printf("> ");
		fgets(buffer, BUFF_SIZE, stdin);
		int len, index; 
		len = strlen(buffer);
		index = hash(buffer);
		buffer[len - 1] = '\0';
		int status = stdout_logic(table, buffer);
		if(status == 1) continue;
		append(&table[index], buffer);
	}
	return 0;
}