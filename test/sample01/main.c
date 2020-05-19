/**
 * sample01: Stack Overflow
 */
#include <stdio.h>

void vuln() {
  char buf[0x10];
  gets(buf);
  puts(buf);
}

int main(int argc, char **argv, char **envp) {
  vuln();
  return 0;
}
