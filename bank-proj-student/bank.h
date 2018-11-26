#ifndef __BANK_H
#define __BANK_H


// The `bank_open` function "opens" a bank with `atm_cnt` ATMs
// attached to it and `account_cnt` accounts created. It must be
// called before the `run_bank` function is called.

void bank_open (int atm_cnt, int account_cnt);


// The `bank_close` function "closes" a bank. The result of closing a
// bank will print the account balances to standard output. This
// function should only be called after a call to `bank_open`.

void bank_close ();


// Print the accounts and their balance.
void bank_dump ();


// The `run_bank` function runs the "bank" which will wait for
// commands from ATM processes. It can only be called after a call to
// `bank_open` is called. This function has the following parameters:
//
// bank_in_fd - A table of input file descriptors from which the bank
//              will receive commands.  Each file descriptor corresponds
//              to a different ATM.  This must have atm_cnt elements.
// atm_out_fd - A table of file descriptors to which the bank can
//              send commands.  Each file descriptor corresponds
//              to a different ATM.  This must have atm_cnt elements.
//
// This function will return a status code indicating success or
// failure. The list of error codes can be found in errors.h.

int run_bank (int bank_in_fd[], int atm_out_fd[]);

#endif
