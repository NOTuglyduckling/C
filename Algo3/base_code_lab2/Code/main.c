// Need this to use the getline C function on Linux. Works without this on MacOs. Not tested on Windows.
#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "token.h"
#include "queue.h"
#include "stack.h"


/** 
 * Utilities function to print the token queues
 */
void print_token(const void* e, void* user_param);
void print_queue(FILE* f, Queue* q);

/** 
 * Function to be written by students
 */

bool issymbol(const char c);
Queue* stringToTokenQueue(const char* expression);
void computeExpressions(FILE* input);



void computeExpressions(FILE* input) {
    char *buf = NULL;
    size_t size = 0;

    while (getline(&buf, &size, input) != -1) {
        printf("Input	: %s", buf);
		Queue* queue = stringToTokenQueue(buf);
		printf("Infix	: ");
		print_queue(stdout,queue);
		printf("\n\n");
		while (!queue_empty(queue)){
			const Token* token = queue_top(queue);
			queue_pop(queue);
			if (token != NULL){
				delete_token((ptrToken*)&token);
			}
		}
		delete_queue((ptrQueue*)&queue);
	}
    free(buf);
}

bool issymbol(const char c){
	char* symbols = "+-*/^()";
	while (*symbols){
		if (*symbols == c)
			return true;
		symbols++;
	}
	return false;
}

Queue* stringToTokenQueue(const char* expression){
	Queue* queue = create_queue();
	const char* curpos = expression;
	while (*curpos != '\0'){
		while ((*curpos == ' '|| *curpos =='\n')&& *curpos != '\0'){
			curpos++;
		}
		if (issymbol(*curpos)){
			Token* t = create_token_from_string(curpos,1);
			queue_push (queue,t);
			curpos++;
		} else {
			int taille = 0;
			while ( curpos[taille] != ' ' && curpos[taille]!='\n' && 
					curpos[taille] != '\0' && !issymbol(curpos[taille]))
				taille++;
            Token* t = create_token_from_string(curpos, taille);
            queue_push(queue, t);
            curpos += taille;
		}	
	}
	return queue;
}

Queue* shuntingYard(Queue* infix){
	Queue* output = create_queue();
	Stack* operators = create_stack();
	while(!queue_empty(infix)){
		Token* token = queue_pop(infix);
		if (token_is_number(token)){
			queue_push(output,token);
		}
		else if (token_is_operator(token)){
			while(!stack_empty(operators) && 
					((operator_precedence(stack_top(operators)) > operator_precedence(token)) ||
            		(operator_precedence(stack_top(operators)) == operator_precedence(token) &&
            		operator_is_left_associative(token))) &&
            		!token_is_left_bracket(stack_top(operators))){
				Token* operator = stack_top(operators);
				stack_pop(operators);
				queue_push(output,operator);
			}
			stack_push(operators,token);
		} else if (token_is_left_bracket(token)){
			stack_push(operators,token);
		} else if (token_is_right_bracket(token)){
			while (!stack_empty(operators) && 
					!token_is_left_bracket(stack_top(operators))){
				Token* operator = stack_top(operators);
				stack_pop(operators);
				queue_push(output,operator);
			}
			if (!stack_empty(operators) && 
				token_is_left_bracket(stack_top(operators))){
				Token* leftBracket = stack_top(operators);
				stack_pop(operators);
				delete_token(leftBracket);
			} else {
				fprintf(stderr,"Error : parenthèse non fermée.");
				return;
			}
		}
	}
	while (!stack_empty(operators)){
		Token* operator = stack_top(operators);
		if (token_is_bracket(operator)){
			fprintf(stderr,"Error : parenthèse non fermée");
			return;
		}
		stack_pop(operators);
		queue_push(output,operator);
	}
	delete_stack(operators);
}

/** Main function for testing.
 * The main function expects one parameter that is the file where expressions to translate are
 * to be read.
 *
 * This file must contain a valid expression on each line
 *
 */
int main(int argc, char** argv){
	if (argc<2) {
		fprintf(stderr,"usage : %s filename\n", argv[0]);
		return 1;
	}
	
	FILE* input = fopen(argv[1], "r");

	if ( !input ) {
		perror(argv[1]);
		return 1;
	}

	computeExpressions(input);

	fclose(input);
	return 0;
}
 
void print_token(const void* e, void* user_param) {
	FILE* f = (FILE*)user_param;
	Token* t = (Token*)e;
	token_dump(f, t);
}

void print_queue(FILE* f, Queue* q) {
	fprintf(f, "(%d) --  ", queue_size(q));
	queue_map(q, print_token, f);
}