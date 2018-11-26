#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "command.h"

// An array of strings that corresponds to each of the command types.
char *cmd_strings[] = {
  "OK",
  "CONNECT",
  "EXIT",
  "DEPOSIT",
  "WITHDRAW",
  "TRANSFER",
  "BALANCE",
  "NOFUNDS",
  "ATMUNKN",
  "ACCUNKN"
};


// converts an int to packed byte form

static void pack_int (unsigned in, byte out[]) {
  out[0] = in >> 24;
  out[1] = in >> 16;
  out[2] = in >>  8;
  out[3] = in;
}


// converts packed byte form to an int

static unsigned unpack_int (byte in[]) {
  return (in[0] << 24) | (in[1] << 16) | (in[2] << 8) | (in[3]);
}

void cmd_pack (Command *cmd, cmd_t c, int id, int from, int to, int amt) {
  cmd->cmd[0] = c;
  pack_int(id  , cmd->id  );
  pack_int(from, cmd->from);
  pack_int(to  , cmd->to  );
  pack_int(amt , cmd->amt );
}

void cmd_unpack (Command *cmd,
                 cmd_t *c, int *id, int *from, int *to, int *amt) {
  *c = cmd->cmd[0];
  *id   = unpack_int(cmd->id  );
  *from = unpack_int(cmd->from);
  *to   = unpack_int(cmd->to  );
  *amt  = unpack_int(cmd->amt );
}

void cmd_dump (char *msg, int id, Command *cmd) {
  // We use an environment variable to toggle debug printing. If the
  // environment variable `BANKSIM_DEBUG` is set then commands will be
  // dumped to standard output.
  char *eval = getenv("BANKSIM_DEBUG");
  if (eval != NULL) {
    cmd_t c;
    int i, f, t, a;
    cmd_unpack(cmd, &c, &i, &f, &t, &a);
    printf("%s[%d] %s %d %d %d %d\n", msg, id, cmd_strings[c], i, f, t, a);
    fflush(stdout);
  }
}
