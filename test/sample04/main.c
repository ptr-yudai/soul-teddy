/**
 * sample04: vtable OOB
 *
 * CHECK
 * - `mov` is properly instrumented
 * - User input from `read` syscall is properly tainted
 */
#include <stdlib.h>
#include <stdio.h>

void func1(long a) { printf("[+] func1(%ld);\n", a); }
void func2(long a) { printf("[+] func2(%ld);\n", a); }

void (*f[])(long) = {func1, func2};
long n, a;

int main(int argc, char **argv, char **envp) {
  printf("Enter 0 or 1: ");
  scanf("%ld", &n);
  printf("Argument: ");
  scanf("%ld", &a);
  f[n](a);
}
