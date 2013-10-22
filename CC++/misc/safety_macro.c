/*
  show how to write a safety marco MAX with a
  typeof keyword.
*/

#include <stdio.h>

/* MAX with side effect */
#define MAX_NSAF(a, b) (a > b ? a : b)

/* make sure that 'a' and 'b' only be evaluate once */
#define MAX_SAF(type, a, b) \
  ({type _a = (a);	     \
    type _b = (b);	     \
    _a > _b ? _a : _b;})
  
/* provide more flexibility using typeof */
#define MAX_SAF2(a, b) \
  ({typeof(a) _a = (a); \
  typeof(b) _b = (b);	\
  _a > _b ? _a : _b;})

int main()
{
  int a = 3, b = 5;
  printf("expected value: %d, returned value: %d\n",
	 b, MAX_NSAF(a, b));

  /* show how ++ operation is evaluate twice */
  printf("expected value: 6, returned value: %d\n",
	 MAX_NSAF(++a, ++b));

  a = 15, b = 11;
  printf("expected value: 16, returned value: %d\n",
	 MAX_SAF(int, ++a, ++b));  

  printf("expected value: 16, returned value: %f\n",
	 MAX_SAF2(a++, 16.0+5));    

  return 0;
}
