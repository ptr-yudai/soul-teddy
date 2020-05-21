/**
 * sample02: Stack Overflow (OOB)
 *
 * CHECK
 * - `ret` is properly instrumented
 * - User input from `read` syscall is properly tainted
 * - Indexing access to an array is properly considered as pointer access
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
