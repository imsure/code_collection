/*
  A client program that tests generic stack.
*/

#include "generic_stack.h"
#include <string.h>

void test_int_stack(void);
void test_string_stack(void);
void str_free(void *obj);
void test_struct_stack(void);

struct foo {
  int *bar;
  char *name;
};

int main()
{
  //  test_int_stack();
  //  test_string_stack();
  test_struct_stack();
  return 0;
}

void test_int_stack(void)
{
  stack int_stack;
  int top;

  stack_new(&int_stack, sizeof(int), NULL);
  
  for (int i = 0; i < 10; i++) {
    stack_push(&int_stack, &i);
  }
  for (int i = 0; i < 10; i++) {
    stack_pop(&int_stack, &top);
    printf("%d ", top);
  }
  putchar('\n');
  stack_dispose(&int_stack);
}

void str_free(void *obj)
{
  printf("%s was called.\n", __FUNCTION__);
  printf("Memory freed: %p\n", *(char **)obj);
  free(*(char **)obj);
}

void test_string_stack(void)
{
  stack str_stack;
  char *top;
  const char *friends[] = {"Ross", "Chandler", "Joy"};

  stack_new(&str_stack, sizeof(char *), str_free);

  for (int i = 0; i < 3; i++) {
    /* Replicate strings in 'friends' on heap and return
       the pointer to it. So it's client's responsibility
       to manage this memory area. */
    char *temp = strdup(friends[i]);
    stack_push(&str_stack, &temp);
  }

  for (int i = 0; i < 3; i++) {
    stack_pop(&str_stack, &top);
    printf("%s\n", top);
  }
  stack_dispose(&str_stack);

  stack_new(&str_stack, sizeof(char *), str_free);
  for (int i = 0; i < 3; i++) {
    /* Replicate strings in 'friends' on heap and return
       the pointer to it. So it's client's responsibility
       to manage this memory area. */
    char *temp = strdup(friends[i]);
    printf("temp: %p\n", temp);
    stack_push(&str_stack, &temp);
  }

  stack_dispose(&str_stack);
}

void foo_free(void *foo)
{
  printf("%s was called.\n", __FUNCTION__);
  free(((struct foo *)foo)->bar);
  free(((struct foo *)foo)->name);
}

void test_struct_stack(void)
{
  const char *friends[] = {"Ross", "Chandler", "Joy"};
  stack struct_stack;

  stack_new(&struct_stack, sizeof(struct foo), foo_free);
  for (int i = 0; i < 3; i++) {
    struct foo f;
    f.bar = malloc((i+5)*sizeof(int));
    f.bar[i] = i;
    f.name = strdup(friends[i]);
    stack_push(&struct_stack, &f);
  }

  #if 0
  for (int i = 2; i >=0 ; i--) {
    struct foo f;
    stack_pop(&struct_stack, &f);
    printf("f.bar[%d] : %d\n", i, f.bar[i]);
    printf("f.name : %s\n", f.name);
  }
  #endif
  stack_dispose(&struct_stack);
}
