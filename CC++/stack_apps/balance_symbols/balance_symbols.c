/*
 * Check for Balancing of Parenthese, Brackets and Braces in the source files.
 * A Very Smart Application of Stack.
 */

#include <stdio.h>
#include <stdlib.h>
#include "linklist_stack.h"

const char * left_symbol  = "{[(";
const char * right_symbol = ")]}";

void
print_error (int symbol, int line_num)
{
	printf ("Symbol Errors:\n");
	printf ("%c at line %d doesn't have a corresponding left symbol.\n", symbol, line_num);
}

int main (int argc, char *argv[])
{
	FILE *fp;
	char c, c1;
	int line_cnt = 0;
	struct stack_node *stack;

	if (argc != 2) {
		printf ("Usage: %s <source file name>\n", argv[0]);
		exit (EXIT_FAILURE);
	}
	fp = fopen (argv[1], "r");
	if (NULL == fp) {
		fatal_error ("Open File Stream Failed !!!");
	}

	stack = create_stack ();

	while ((c = fgetc(fp)) != EOF) {
		if ('\n' == c) {
			line_cnt++;
			continue;
		}
		if ((left_symbol[0] == c) || (left_symbol[1] == c) || (left_symbol[2] == c)) {
			push (stack, c);
		} else if ((right_symbol[0] == c) || (right_symbol[1] == c) || (right_symbol[2] == c)) {
			if (is_empty(stack)) {
				print_error (c, line_cnt);
				goto QUIT;
			} else {
				c1 = pop (stack);
				switch (c) {
				case ')' :
					if (c1 != '(')	
						print_error (c, line_cnt);
					break;
				case ']' :
					if (c1 != '[')	
						print_error (c, line_cnt);
					break;
				case '}' :
					if (c1 != '{')	
						print_error (c, line_cnt);
					break;
				default:	break;
				}
			}
		}
	}

	if (!is_empty(stack)) {
		printf ("End of FIle. Stack is not empty. Error Occurs.\n");
		printf ("Total line of file is %d\n", line_cnt);
	}

QUIT:
	return 0;
}

