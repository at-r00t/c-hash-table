#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashlib.h"

#define T_LEN 10
#define BUFF_SIZE 4096

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
		buffer[len - 1] = '\0';
		if(strcmp(buffer ,"\0") == 0)continue;
		int status = logic(table, T_LEN, buffer);
		if(status == 1) continue;
		
		index = hash(buffer, T_LEN);
		append(&table[index], buffer);
	}
	return 0;
}