#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "hw.h"

// Assumes 0 <= range <= RAND_MAX
// Returns in the half-open interval [0, max]
int random_at_most (long max) {
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  int x;
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)(x = rand()));

  // Truncated division is intentional
  return x/bin_size;
}

int main (int argc, char *argv[]) {
  if (argc != 4) {
    printf("usage: %s atm_cnt account_cnt trans_cnt\n", argv[0]);
    exit(1);
  }

  // Get the number of ATMs from command line.
  int atm_cnt;
  sscanf(argv[1], "%d", &atm_cnt);

  // Get the number of accounts from command line.
  int account_cnt;
  sscanf(argv[2], "%d", &account_cnt);

  // Get the number of transactions from command line.
  int trans_cnt;
  sscanf(argv[3], "%d", &trans_cnt);

  // Create trace file name.
  char file[200];
  sprintf(file, "%d_%d_%d.trace", atm_cnt, account_cnt, trans_cnt);
  
  // Delete trace file if it already exists.
  remove(file);
  
  // Open trace file for writing.
  int tfd = open(file, O_CREAT | O_WRONLY, 0666);

  // First, write the number of ATMs and accounts.
  byte atms_buf[4]; 
  memset(atms_buf, 0, 4);
  atms_buf[0] |= (atm_cnt >> 24);
  atms_buf[1] |= (atm_cnt >> 16) & 0xFF;
  atms_buf[2] |= (atm_cnt >> 8) & 0xFF;
  atms_buf[3] |= atm_cnt & 0xFF;
  write(tfd, atms_buf, 4);

  byte accts_buf[4];
  memset(accts_buf, 0, 4);
  accts_buf[0] |= (account_cnt >> 24);
  accts_buf[1] |= (account_cnt >> 16) & 0xFF;
  accts_buf[2] |= (account_cnt >> 8) & 0xFF;
  accts_buf[3] |= account_cnt & 0xFF;
  write(tfd, accts_buf, 4);

  // The command buffer.
  Command cmd;

  // Next, have all the atms connect.
  for (int i = 0; i < atm_cnt; i++) {
    MSG_CONNECT(&cmd, i);
    cmd_dump("twriter", 0, &cmd);
    write(tfd, &cmd, MESSAGE_SIZE);
  }

  // Next, all accounts will deposit starter cash.
  for (int i = 0; i < account_cnt; i++) {
    int rand_atm  = random_at_most(atm_cnt-1);
    MSG_DEPOSIT(&cmd, rand_atm, i, 5000);
    cmd_dump("twriter", 0, &cmd);
    write(tfd, &cmd, MESSAGE_SIZE);
  }

  // Next, we randomly generate transactions.
  for (int i = 0; i < trans_cnt; i++) {
    int rand_atm       = random_at_most(atm_cnt-1);
    int rand_from_acct = random_at_most(account_cnt-1);
    int rand_to_acct   = random_at_most(account_cnt-1);
    int rand_amount    = random_at_most(200);
    cmd_t rand_trans   = random_at_most(2);
    switch (rand_trans+4) {
    case TRANSFER:
      MSG_TRANSFER(&cmd, rand_atm, rand_from_acct, rand_to_acct, rand_amount);
      break;
    case DEPOSIT:
      MSG_DEPOSIT(&cmd, rand_atm, rand_to_acct, rand_amount);
      break;
    case WITHDRAW:
      MSG_WITHDRAW(&cmd, rand_atm, rand_from_acct, rand_amount);
      break;
    case BALANCE:
      MSG_BALANCE(&cmd, rand_atm, rand_from_acct);
      break;      
    }
    cmd_dump("twriter", 0, &cmd);
    write(tfd, &cmd, MESSAGE_SIZE);
  }

  // Lastly, we generate exits.
  for (int i = 0; i < atm_cnt; i++) {
    MSG_EXIT(&cmd, i);
    cmd_dump("twriter", 0, &cmd);
    write(tfd, &cmd, MESSAGE_SIZE);
  }

  close(tfd);

  return 0;
}
