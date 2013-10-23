/*
 * Converse Infix Expression to Postfix Expression
 * An Application of Stack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linklist_stack.h"

enum {OPERATOR, OPERAND};

enum {
	PRIORITY_PLUS_MINUS = 0,
	PRIORITY_MULT_DIV,
	PRIORITY_LEFT_PARENTHESIS
};

enum {
	X_VAR = 0,
	Y_VAR,
	Z_VAR
};

const char tokens[] = "+-*/()"; 

void check_priority ();
void infix2postfix (const char *in, char *out);

int main (int argc, char *argv[])
{
	char *infix;
	char *postfix;

	if (argc != 2) {
		printf ("Usage: %s <expression>\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	postfix = (char *)malloc(strlen(argv[1])+1);
	if (NULL == postfix) {
		fatal_error ("Out of Space !!!");
	}
	infix = (char *)malloc(strlen(argv[1])+1);
	if (NULL == infix) {
		fatal_error ("Out of Space !!!");
	}
	strcpy (infix, argv[1]);
	printf ("input expression is:\n%s\n", infix);
	
	infix2postfix (infix, postfix);
	printf ("out expression:\n%s\n", postfix);

	return 0;
}

int 
check_symbols (char sym)
{
	int i;
	for (i=0; i<=strlen(tokens); i++) {
		if (sym == tokens[i])
			return OPERATOR;
	}
	return OPERAND;
}

int 
get_priority (char ch)
{
	if (ch == '+' || ch == '-')
		return PRIORITY_PLUS_MINUS;
	else if (ch == '*' || ch == '/')
		return PRIORITY_MULT_DIV;
	else if (ch == '(')
		return PRIORITY_LEFT_PARENTHESIS;
}

void 
infix2postfix (const char *in, char *out)
{
	int i = 0;
	char ch, top_entry;
	struct stack_node * stack;

	stack = create_stack ();
	if (NULL == stack) {
		fatal_error ("Out of Space !!!");
	}

	while ((ch = *in++) != '\0') {
		if ((check_symbols(ch) == OPERAND)) {
			// simply put operand to the output sequence
			out[i++] = ch;	
		} else {	// tackle with operators
			if (is_empty(stack)) {
				push (stack, ch);
			} else if (')' != ch) {
				while (!is_empty(stack)) {
					top_entry = pop (stack);
					if (get_priority(ch) > get_priority(top_entry)) {
						push (stack, top_entry);
						push (stack, ch);
						break;
					} else if (get_priority(top_entry) != PRIORITY_LEFT_PARENTHESIS) {
						out[i++] = top_entry;
					} else if (get_priority(top_entry) == PRIORITY_LEFT_PARENTHESIS) {
						push (stack, top_entry);
						push (stack, ch);
						break;
					}
				}
				if (is_empty(stack)) {
					push (stack, ch);
				}
			} else if (')' == ch) {
				// pop until meet '(' in stack
				while ((top_entry = pop(stack)) != '(') {
					out[i++] = top_entry;
				}
			}
		}
	}

	while (!is_empty(stack)) {
		top_entry = pop (stack);
		out[i++] = top_entry;
	}

	// free stack space 
	delete_stack (stack);
}

# if 0
void 
cal_expr (const char *expr, int *val_table);
{
	char ch, c_opd1, c_opd2;
	int i_opd1, i_opd2, res;
	struct stack_node * stack;

	stack = create_stack ();
	if (NULL == stack) {
		fatal_error ("Out of Space !!!");
	}

	while ((ch = *expr++) != '\0') {
		if ((check_symbols(ch) == OPERAND)) {
			// as long as it is an operand, push to stack, wating to calculate
			push (stack, ch);
		} else {	// dispose operators 
			c_opd1 = pop (stack);
			c_opd2 = pop (stack);
			i_opd1 = char2val (c_opd1, val_table);
			i_opd2 = char2val (c_opd2, val_table);
			res = calculator (i_opd1, i_opd2, ch);
			
		}
	}
}

int 
char2val (char c, int *val_table)
{
	int val;

	switch (c) {
	case 'x':	val = val_table[X_VAR];	break;
	case 'y':	val = val_table[Y_VAR];	break;
	case 'z':	val = val_table[Z_VAR];	break;
	default:	break;
	}

	return val;
}

int 
calculator (int opd1, int opd2, char operator)
{
	int res;

	switch (operator) {
	case '+':	res = opd1 + opd2;	break;	
	case '*':	res = opd1 * opd2;	break;	
	case '-':	res = opd2 - opd1;	break;	
	case '/':	if (opd1 != 0) { res = opd2 / opd1;	break; }
				else { printf ("Divident is zero !!!\n"); exit (EXIT_FAILURE); }
	default:	break;
	}

	return res;
}

#endif
