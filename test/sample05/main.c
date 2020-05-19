/**
 * sample05: off-by-null
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void vuln(int *pn) {
  char buf[0x80];
  int s = read(0, buf, 0x80);
  buf[s] = '\0';
  printf("[+] str: %s", buf);
  *pn = atoi(buf);
}

int main(int argc, char **argv, char **envp) {
  int n;
  vuln(&n);
  printf("[+] int: %d\n", n);
}
