#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "atm.h"
#include "command.h"
#include "trace.h"
#include "errors.h"

// The following functions should be used to read and write
// groups of data over the pipes.  Use them in the implementation
// of the `atm` function below!

// Performs a `write` call, checking for errors and handlings
// partial writes. If there was an error it returns ERR_PIPE_WRITE_ERR.
// Note: data is void * since the actual type being written does not matter.

static int checked_write (int fd, void *data, int n) {
  while (n > 0) {
    int result = write(fd, data, n);
    if (result >= 0) {
      // this approach handles both complete and partial writes
      data += result;
      n    -= result;
    } else {
      error_msg(ERR_PIPE_WRITE_ERR,
                "could not write message to bank");
      return ERR_PIPE_WRITE_ERR;
    }
  }
  return SUCCESS;
}

// Performs a `read` call, checking for errors and handlings
// partial read. If there was an error it returns ERR_PIPE_READ_ERR.
// Note: data is void * since the actual type being read does not matter.

static int checked_read (int fd, void *data, int n) {
  while (n > 0) {
    int result = read(fd, data, n);
    if (result >= 0) {
      // this approach handles both complete and partial reads
      data += result;
      n    -= result;
    } else {
      error_msg(ERR_PIPE_READ_ERR,
                "could not read message from bank");
      return ERR_PIPE_READ_ERR;
    }
  }
  return SUCCESS;
}

// The `atm` function processes commands received from a trace
// file.  It communicates to the bank transactions with a matching
// ID.  It then receives a response from the bank process and handles
// the response appropriately.

int atm (int bank_out_fd, int atm_in_fd, int atm_id, Command *cmd) {
  byte c;
  int i, f, t, a;
  Command atmcmd;

  cmd_unpack(cmd, &c, &i, &f, &t, &a);

  int status = SUCCESS;

  // TODO:
  // START YOUR IMPLEMENTATION

if(atm_id == i){

      if(c==CONNECT ||
	 c==EXIT ||
	 c ==DEPOSIT ||
	 c==TRANSFER ||
	 c==BALANCE ||
	 c== WITHDRAW){

      status = checked_write(bank_out_fd,cmd,MESSAGE_SIZE);
      	if(status != SUCCESS){
        	return status;
      	}
      status= checked_read(atm_in_fd,&atmcmd,MESSAGE_SIZE);
        if(status != SUCCESS){
         	return status;
        }

          cmd_unpack(&atmcmd,&c, &i, &f, &t, &a);
          
            if(c == OK){
              status = SUCCESS;
            }
            if(c == NOFUNDS){
              status = ERR_NOFUNDS;
            }
            if(c == ACCUNKN){
              status = ERR_UNKNOWN_ACCOUNT;
            }
          }
      
          else{
            return ERR_UNKNOWN_CMD;
          }
     }
     
  else{
  	status = ERR_UNKNOWN_ATM;
}
  
  // END YOUR IMPLEMENTATION

  return status;
}

int atm_run (char *trace, int bank_out_fd, int atm_in_fd, int atm_id) {
  int status = trace_open(trace);
  if (status == -1) {
    error_msg(ERR_BAD_TRACE_FILE, "could not open trace file");
    return ERR_BAD_TRACE_FILE;
  }

  Command cmd;
  while (trace_read_cmd(&cmd)) {
    status = atm(bank_out_fd, atm_in_fd, atm_id, &cmd);

    switch (status) {

    // We continue if the ATM was unknown. This is ok because the trace
    // file contains commands for all the ATMs.
    case ERR_UNKNOWN_ATM:
      break;

    // We display an error message to the ATM user if the account
    // is not valid.
    case ERR_UNKNOWN_ACCOUNT:
      printf("ATM error: unknown account! ATM Out of service\n");
      break;

    // We display an error message to the ATM user if the account
    // does not have sufficient funds.
    case ERR_NOFUNDS:
      printf("not enough funds, retry transaction\n");
      break;

    // If we receive some other status that is not successful
    // we return with the status.
    default:
      if (status != SUCCESS) {
	printf("status is %d\n", status);
	return status;
      }
    }
  }

  trace_close();

  return SUCCESS;
}
