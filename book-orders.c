#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer-database.h"
//MAIN PROG ARGS: ./book-orders <cust database input file> <book order input file> <categories files>

int main(int argc, char **argv){
	char *cust_file = argv[1];
	char *order_file = argv[2];
	char *categories_file = argv[3];
	
	printf("customer file: %s\n", cust_file);
	printf("order file: %s\n", order_file);
	printf("categories file: %s\n", categories_file);
	printf("\n");
	
	read_customers(cust_file);
	return 0;
}

int read_customers(char *filename){
	FILE *customer_file;
	customer_file = fopen("/ilab/users/fs263/Desktop/cs214/pa5/database.txt", "r");
	
	if (customer_file == NULL){
    		printf("ERR: could not read file %s", filename);
    		return -1;
	}
	
	char line[300]; //remember MAY NEED TO MODIFY
	const char delims[2] = "|";
	
	while(fgets(line, 300, customer_file) != NULL){ //stored in 'line'
		printf("line: %s", line);
		
		Customer *cust =(Customer *) malloc(sizeof(Customer));
		char *token;
		token = strtok(line, delims);
		int item = 0; //tells you which attribute of the customer is being observed
		
		while(token != NULL){
			//printf("  %s\n", token);
			token = strtok(NULL, delims);
			char *value =(char *) malloc(sizeof(strlen(token)));
			memset(value, '\0', sizeof(value));
   			strcpy(value, token);
			printf("  %s\n", value);
			if(item == 0){ //name, string
				cust->name = token;
			}else if(item == 1){ //customer id, int
				//cust->id = token; //CONVERT TO INT****
			}else if(item == 2){  //customer credit, double
				//cust->credit = token; //CONVERT TO DOUBLE****
			}else if(item == 3){ //street address, string
				cust->address = token;
			}else if(item == 4){ //state, string
				cust->state = token;
			}else if(item == 5){ //zip, string
				cust->zip = token;
			}
			item++;
		}
	}
	return 0;
}
