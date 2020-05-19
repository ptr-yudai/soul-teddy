/**
 * sample02: Stack Overflow (OOB)
 */
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  long i, array[4];
  printf("index: ");
  scanf("%ld", &i);
  printf("value: ");
  scanf("%ld", &array[i]);
  return 0;
}
