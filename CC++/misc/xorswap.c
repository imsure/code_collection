/*
  xor swap algorithm:
  R1 <-- a
  R2 <-- b
  R1 <-- a xor b
  R2 <-- R1 xor R2 = (a xor b) xor b
                   = a xor (b xor b)
		   = a xor 0
		   = a
  R1 <-- R1 xor R2 = (a xor b) xor a
                   = b xor (a xor a)
		   = b xor 0
		   = b

 Note: this algorithm is useful in practice in
 modern cpu architechure. for details see:
 http://en.wikipedia.org/wiki/XOR_swap_algorithm
*/

#include <stdio.h>
#include <stdbool.h>

#define SWAP_V1(a, b) {a ^= b; b ^= a; a ^= b;}
#define SWAP_V2(a, b) do { \
    a ^= b; b ^= a; a ^= b; \
  }while (0)

int main()
{
  int x = 3, y = 7;

  SWAP_V1(x, y);
  printf("x = %d, y = %d\n", x, y);

  
#if 0
  /* following code won't be compiled because
   the semicolon after the closing curly brace
   will break the flow between if and else. */
  if (true)
    SWAP_V1(x, y);
  else
    SWAP_V1(y, x);
#endif

  if (true)
    SWAP_V2(x, y);
  else
    SWAP_V2(y, x);

  printf("x = %d, y = %d\n", x, y);
}
