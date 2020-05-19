/**
 * sample04: vtable OOB
 */
#include <stdlib.h>
#include <stdio.h>

void func1(long a) { printf("[+] func1(%ld);\n", a); }
void func2(long a) { printf("[+] func2(%ld);\n", a); }

void (*f[])(long) = {func1, func2};

int main(int argc, char **argv, char **envp) {
  int n = 0;
  long a = 0;
  printf("Enter 0 or 1: ");
  scanf("%d", &n);
  printf("Argument: ");
  scanf("%ld", &a);
  f[n](a);
}
