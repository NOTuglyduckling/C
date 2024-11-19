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
Queue* shuntingYard(Queue* infix);
Token* evaluateOperator(Token* arg1,Token* op,Token* arg2);
float evaluateExpression(Queue* postfix);



void computeExpressions(FILE* input) {
    char *buf = NULL;
    size_t size = 0;

    while (getline(&buf, &size, input) != -1) {
        printf("Input	 : %s", buf);
		Queue* Infix = stringToTokenQueue(buf);
		printf("Infix	 : ");
		print_queue(stdout,Infix);
		printf("\n");
		Queue* Postfix = shuntingYard(Infix);
		printf("Postfix  : ");
		print_queue(stdout,Postfix);
		printf("\n");
		float result = evaluateExpression(Postfix);
		printf("Evaluate : %f\n\n",result);
		while (!queue_empty(Infix)){
			const Token* token = queue_top(Infix);
			queue_pop(Infix);
			if (token != NULL){
				delete_token((ptrToken*)&token);
			}
		}
		delete_queue((ptrQueue*)&Infix);
		while (!queue_empty(Postfix)){
			const Token* token = queue_top(Postfix);
			queue_pop(Postfix);
			if (token != NULL){
				delete_token((ptrToken*)&token);
			}
		}
		delete_queue((ptrQueue*)&Postfix);
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
		while ((*curpos == ' '|| *curpos =='\n')){
			curpos++;
		}
		if (issymbol(*curpos) && *curpos != '\0'){
			Token* t = create_token_from_string(curpos,1);
			queue_push (queue,t);
			curpos++;
		} else if (*curpos != '\0'){
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
	Stack* operators = create_stack(queue_size(infix));
	while(!queue_empty(infix)){
		const Token* token = queue_top(infix);
		queue_pop(infix);
		if (token_is_number(token)){
			queue_push(output,token);
		}
		else if (token_is_operator(token)){
			while(!stack_empty(operators) && 
					token_is_operator(stack_top(operators))&&
					((token_operator_priority(stack_top(operators)) > token_operator_priority(token)) ||
            		(token_operator_priority(stack_top(operators)) == token_operator_priority(token) &&
            		token_operator_leftAssociative(token))) &&
					(!token_is_parenthesis(stack_top(operators)) || (token_is_parenthesis(stack_top(operators))&&
            		token_parenthesis(stack_top(operators))!='('))){
				const Token* operator = stack_top(operators);
				stack_pop(operators);
				queue_push(output,operator);
			}
			stack_push(operators,token);
		} else if (token_is_parenthesis(token)){
			if (token_parenthesis(token)=='('){
				stack_push(operators,token);
			} else {
				while (!stack_empty(operators) && 
						(!token_is_parenthesis(stack_top(operators)) || 
						(token_is_parenthesis(stack_top(operators))&&
						token_parenthesis(stack_top(operators))!='('))){
					const Token* operator = stack_top(operators);
					stack_pop(operators);
					queue_push(output,operator);
				}
				if (!stack_empty(operators) && 
						token_is_parenthesis(stack_top(operators))&&
						token_parenthesis(stack_top(operators))=='('){
					const Token* leftBracket = stack_top(operators);
					stack_pop(operators);
					delete_token((ptrToken*)&leftBracket);
				} else {
					fprintf(stderr,"Error : parenthèse non fermée.");
				}
			}
		}
	}
	while (!stack_empty(operators)){
		const Token* operator = stack_top(operators);
		if (token_is_parenthesis(operator)){
			fprintf(stderr,"Error : parenthèse non fermée");
		}
		stack_pop(operators);
		queue_push(output,operator);
	}
	delete_stack((ptrStack*)&operators);
	return output;
}

float evaluateExpression(Queue* postfix){
	Stack* stack = create_stack(queue_size(postfix));
	while(!queue_empty(postfix)){
		const Token* current = queue_top(postfix);
		queue_pop(postfix);
		if (token_is_number(current)) {
            printf("Pushing number to stack: %f\n", token_value(current));
            stack_push(stack, (Token*)current);
        } else if (token_is_operator(current)){
			if (stack_size(stack) < 2) {
                fprintf(stderr, "Error: Quantité insuffisante d'opérandes.\n");
                delete_stack((ptrStack*)&stack);
                return 0.0;
            }
			Token* arg2 = (Token*) stack_top(stack);
			stack_pop(stack);
			Token* arg1 = (Token*) stack_top(stack);
			stack_pop(stack);
			Token* value =  evaluateOperator((Token*)arg1,(Token*)current,(Token*)arg2);
			stack_push(stack,value);
			printf("Applying operator '%c' to operands %f and %f\n", token_operator(current), token_value(arg1), token_value(arg2));
			delete_token(&arg1);
            delete_token(&arg2);
		}
	}
	printf("here");
	const Token* endvalue = stack_top(stack);
	float result = token_value(endvalue);
	stack_pop(stack);
	delete_token((ptrToken*)endvalue);
	delete_stack((ptrStack*)&stack);
	return result;
}

Token* evaluateOperator(Token* arg1,Token* op,Token* arg2){
	char operator = token_operator(op);
	float val1 = token_value(arg1);
	float val2 = token_value(arg2);
	switch (operator) {
        case '+':
            return create_token_from_value(val1 + val2);
        case '-':
            return create_token_from_value(val1 - val2);
        case '*':
            return create_token_from_value(val1 * val2);
        case '/':
            if (val2 == 0) {
                fprintf(stderr, "Error: Division par zero.\n");
                return create_token_from_value(0);
            }
            return create_token_from_value(val1 / val2);
        case '^':
            return create_token_from_value(pow(val1, val2));
        default:
            fprintf(stderr, "Error: Operateur inconnu '%c'.\n", operator);
            return create_token_from_value(0);
    }
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