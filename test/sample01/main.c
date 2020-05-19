/**
 * sample01: Stack Overflow
 */
#include <stdio.h>

int main(int argc, char **argv, char **envp) {
  char buf[0x10];
  gets(buf);
  puts(buf);
  return 0;
}
