#include <string.h>
#include <stdio.h>
#include "errors.h"

// An array of strings containing the string representation of each of
// the error codes.
char *error_strings[] = {
  "SUCCESS",
  "ERR_UNKNOWN_CMD",
  "ERR_PIPE_READ_ERR",
  "ERR_PIPE_WRITE_ERR",
  "ERR_UNKNOWN_ACCOUNT",
  "ERR_UNKNOWN_ATM",
  "ERR_BAD_TRACE_FILE",
  "ERR_NOFUNDS",
  "ERR_ATM_CLOSED"
};

// The maximum size of the error character buffer.
#define ERROR_BUFFER_SIZE 200

// Holds the current error code.
static int err = 0;

// Holds the current error message.
static char errm[ERROR_BUFFER_SIZE];

void error_msg (int error, char *msg) {
  err = error - 1;
  strncpy(errm, msg, ERROR_BUFFER_SIZE);
  errm[ERROR_BUFFER_SIZE-1] = '\0';
}

const char *error_msg_str () {
  return errm;
}

int error_type () {
  return err + 1;
}

void error_print () {
  printf("ERROR %d %s\n", err + 1, error_strings[err]);
}
