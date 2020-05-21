/**
 * sample03: BSS Overflow (OOB)
 *
 * CHECK
 * - `call` (memory) is properly instrumented
 * - User input from `read` syscall is properly tainted
 * - GOT is in range of taint targets
 */
#include <stdlib.h>
#include <stdio.h>

long i, array[4];

int main(int argc, char **argv, char **envp) {
  printf("index: ");
  scanf("%ld", &i);
  printf("value: ");
  scanf("%ld", &array[i]);
  exit(0);
}
