#include <stdio.h>
#include <stdlib.h>
#include "hw.h"

int main (int argc, char *argv[]) {
  if (argc != 2) {
    printf("usage: %s trace_file\n", argv[0]);
    exit(1);
  }

  int result = trace_open(argv[1]);
  if (result == -1) {
    printf("%s: could not open %s\n", argv[0], argv[1]);
    exit(1);
  }

  printf("number of ATMs: %d\n", trace_atm_count());
  printf("number of accounts: %d\n", trace_account_count());

  Command cmd;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_dump("TREADER", 0, &cmd);
  }

  trace_close();
  
  return 0;
}
