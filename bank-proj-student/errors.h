#ifndef __ERRORS_H
#define __ERRORS_H

// These are a list of the status codes that can be returned from the
// `run_bank` and `run_atm` functions. The first status code
// represents "success" and the rest correponds to specific error
// conditions that must be handled in the bank and ATM
// implementations.
#define SUCCESS             1
#define ERR_UNKNOWN_CMD     2
#define ERR_PIPE_READ_ERR   3
#define ERR_PIPE_WRITE_ERR  4
#define ERR_UNKNOWN_ACCOUNT 5
#define ERR_UNKNOWN_ATM     6
#define ERR_BAD_TRACE_FILE  7
#define ERR_NOFUNDS         8
#define ERR_ATM_CLOSED      9

// This function is used to record an error and an associated message. It is
// called by the "bank" and "atm" code to indicate any unexpected errors.
void error_msg (int error, char *msg);

// This function is used to retrieve the error code set by `error_msg`.
int error_type ();

// This function is used to retrieve the error message set by `error_msg`.
const char *error_msg_str ();

// This function is used to print an error message set by `error_msg`.
void error_print ();

#endif
