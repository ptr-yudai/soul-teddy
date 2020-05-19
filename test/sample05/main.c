/**
 * sample05: off-by-null
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void internal_vuln(int *pn) {
  char buf[0x100];
  buf[read(0, buf, 0x100)] = '\0';
  printf("[+] str: %s", buf);
  *pn = atoi(buf);
}

void vuln() {
  int n;
  internal_vuln(&n);
  printf("[+] int: %d\n", n);
}

int main(int argc, char **argv, char **envp) {
  vuln();
}
