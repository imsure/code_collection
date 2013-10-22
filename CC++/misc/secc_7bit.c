/*
  Generate a 7-bit Single Error Correcting Code.
  Write for exercise 2.3.13 of the book: Foundation of CS
  
  Problem description:
  If no two strings in a code differ in fewer than 
  three positions, then we can actually correct a 
  single error, by finding the unique string in the 
  code that differs from the received string in only 
  one position. It turns out that there is a code of 
  7-bit strings that corrects single errors and 
  contains 16 strings. Find such a code.
 */

#include <stdio.h>
#include <string.h>

#define BIT_STR_LEN 7
#define MSG_LEN     4
#define TRUE  1
#define FALSE 0

void search_string ();
int compare_bit_str (unsigned char bs1, unsigned char bs2);
void print_bitstr (unsigned char bs);

int main ()
{
  search_string ();
}

void search_string ()
{
  char i, j, k;
  unsigned char max_msg = (1 << MSG_LEN) - 1;
  unsigned char max_chk = (1<<(BIT_STR_LEN - MSG_LEN)) - 1;
  unsigned char bit_str[1 << MSG_LEN];
  int res;

  printf ("max msg is %d\nmax chk is %d\n", max_msg, max_chk);

  /* start with 000,0000 */
  bit_str[0] = 0;

  /* evaluate each message */
  for (i=1; i<=max_msg; i++) {
    bit_str[i] = i;
    /* check current message against previous ones */
    for (k=0; k<=max_chk; k++) {
      /* attach 3-check-bit with 4-message-bit */
      bit_str[i] += k << 4;
      for (j=0; j<i; j++) {
	res = compare_bit_str (bit_str[i], bit_str[j]);
	if (res == TRUE) continue;
	else             break;
      }
      if (res == TRUE) break;
      else             continue;
    }
    if (res == FALSE) {
      printf ("Cannot find a right bit-string.\n");
      return;
    }
  }

  for (i=0; i<=max_msg; i++) {
    print_bitstr (bit_str[i]);
  }
}

int compare_bit_str (unsigned char bs1, unsigned char bs2) 
{
  unsigned char i, shift, diff_cnt = 0;

  for (i=0; i<BIT_STR_LEN; i++) {
    shift = 0x1 << i;
    if ((bs1 & shift) != (bs2 & shift))
      diff_cnt++;
  }
  if (diff_cnt >= 3)
    return TRUE;
  else
    return FALSE;
}

void print_bitstr (unsigned char bs)
{
  int j;
  bs &= ~(0x1 << BIT_STR_LEN); /* set MSB to 0 */
  for (j=BIT_STR_LEN-1; j>=0; j--) {
    printf ("%d-", (bs & (0x1<<j)) >> j);
  }
  printf ("\n");
}
