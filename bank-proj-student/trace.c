#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "command.h"
#include "trace.h"

static int tracefd     = -1;
static int atm_cnt     = 0;
static int account_cnt = 0;

int trace_open (char *path) {
  tracefd = open(path, O_RDONLY);
  if (tracefd == -1)
    return -1;
  // First read in the ATM count.
  int bytes_read;
  byte buf[4];
  bytes_read = read(tracefd, buf, 4);
  if (bytes_read == 4) {
    atm_cnt |= (buf[0] << 24);
    atm_cnt |= (buf[1] << 16);
    atm_cnt |= (buf[2] <<  8);
    atm_cnt |= buf[3];
  } else {
    return -1;
  }

  // Second read in the account count.
  bytes_read = read(tracefd, buf, 4);
  if (bytes_read == 4) {
    account_cnt |= (buf[0] << 24);
    account_cnt |= (buf[1] << 16);
    account_cnt |= (buf[2] <<  8);
    account_cnt |= buf[3];
  } else {
    return -1;
  }

  return 1;
}

void trace_close () {
  assert(tracefd != -1);
  close(tracefd);
  tracefd     = -1;
  atm_cnt     = 0;
  account_cnt = 0;
}

int trace_atm_count () {
  return atm_cnt;
}

int trace_account_count () {
  return account_cnt;
}


int trace_read_cmd (Command *cmd) {
  return read(tracefd, cmd, MESSAGE_SIZE);
}
