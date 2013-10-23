/*
  Implementation of generic linear search.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*
  Description:
  Function used to perform generic linear search.

  Arguments:
  array:
    generic pointer points to an array (a list of elements
    with the same type, e.g. int, float, char *, etc.);
  key:
    generic pointer points to key to be searched in the 'array';
  element_size:
    size of element in 'array', used to move pointer;
  element_number:
    number of elements in 'array';
  cmp_func:
    function pointer points to function used to
    compare 'key' with element in 'array'.

  Return:
  -1 if no match is found;
  index of the matched element if a match is found.
 */
int generic_lsearch(void *array,
		    void *key,
		    size_t element_size,
		    size_t element_number,
		    int (*cmp_func)(void *, void *))
{
  assert(array && key && cmp_func);

  int i;
  for (i = 0; i < element_number; i++) {
    if (cmp_func(array, key) == 0) {
      return i;
    } else {
      array = (char *)array + element_size;
    }
  }

  /* no match found */
  return -1;
}

/* function used to compare int type,
   return the difference between the two */
int cmp_int(void *p1, void *p2)
{
  return *((int *)p1) - *((int *)p2);
}

/* function used to compare float number. */
int cmp_float(void *p1, void *p2)
{
  float cmp_result = *((float *)p1) - *((float *)p2);
  if (cmp_result != 0.0) return 1;
  else return 0;
}

/* function used to compare string */
int cmp_str(void *p1, void *p2)
{
  // char *str1 = ((char **)p1)[0];
  /* same with statement above */
  char *str1 = *(char **)p1;
  char *str2 = p2;

  printf("str1: %s, str2: %s\n", str1, str2);

  return strcmp(str1, str2);
}

int main()
{
  int search_result;
  int array_int[5] = {1, 2, 3, 4, 5};
  int key_int = 0;
  float array_float[5] = {3.33, 4.44, 5.55, 6.66, 7.77};
  float key_float = 3.32999999999999999;
  char *array_str[5] = {"abc", "123", "shuo", "Yang", "NULL"};
  char *key_str = "abc";

  printf ("%p\n", array_str);
  printf ("%p\n", array_str[0]);
  printf ("%p\n", array_str[1]);

  /* integer comparsion test */
  search_result = generic_lsearch(array_int, &key_int,
				  sizeof(int), 5,
				  cmp_int);
  search_result == -1 ? printf("not found\n") : printf("found\n");

  /* float number comparsion test */
  search_result = generic_lsearch(array_float, &key_float,
				  sizeof(float), 5,
				  cmp_float);
  search_result == -1 ? printf("not found\n") : printf("found\n");

  /* string comparsion test1 : initialized array,
     all strings stored as constants in sequential global area. */
  search_result = generic_lsearch(array_str, key_str,
				  sizeof(char *), 5,
				  cmp_str);
  search_result == -1 ? printf("not found\n") : printf("found\n");

  /* string comparsion test2 : uninitialized array,
     all strings are dynamically allocated and may have
     discontiguous address. */
  char *array_str_dy[5];
  array_str_dy[0] = strdup("I");
  array_str_dy[1] = strdup("am");
  array_str_dy[2] = strdup("a");
  array_str_dy[3] = strdup("student");
  array_str_dy[4] = strdup(".\n");
  char *key_str_dy = strdup(".\n");
  search_result = generic_lsearch(array_str_dy, key_str_dy,
				  sizeof(char *), 5,
				  cmp_str);
  search_result == -1 ? printf("not found\n") : printf("found\n");

  return 0;
}
