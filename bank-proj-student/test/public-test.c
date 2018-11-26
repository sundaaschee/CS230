#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
// Include the check header file:
#include <check.h>

// Include assignment header file:
#include "../hw.h"

//  For more details see http://check.sourceforge.net

// The following definitions are included to test the internal
// implementation of the atm and bank components:

int atm (int bank_out_fd, int atm_in_fd, int atm_id, Command *cmd);
int bank (int atm_out_fd[], Command *cmd, int *atm_cnt);
int *get_accounts ();

//////////////////////////////////////////////////////////////////////
///////////// errors unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_error_msg_SUCCESS)
{
  char *msg = "this is successful";
  int  type = SUCCESS;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

START_TEST(test_error_msg_ERR_UNKNOWN_CMD)
{
  char *msg = "unknown command error";
  int  type = ERR_UNKNOWN_CMD;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

START_TEST(test_error_msg_ERR_PIPE_READ_ERR)
{
  char *msg = "pipe read error";
  int  type = ERR_PIPE_READ_ERR;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

START_TEST(test_error_msg_ERR_PIPE_WRITE_ERR)
{
  char *msg = "pipe write error";
  int  type = ERR_PIPE_WRITE_ERR;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

START_TEST(test_error_msg_ERR_UNKNOWN_ACCOUNT)
{
  char *msg = "unknown account error";
  int  type = ERR_UNKNOWN_ACCOUNT;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

START_TEST(test_error_msg_ERR_UNKNOWN_ATM)
{
  char *msg = "unknown atm error";
  int  type = ERR_UNKNOWN_ATM;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

START_TEST(test_error_msg_ERR_BAD_TRACE_FILE)
{
  char *msg = "bad trace file error";
  int  type = ERR_BAD_TRACE_FILE;
  error_msg(type, msg);
  ck_assert_msg(error_type() == type, "error_type() should return %d", type);
  ck_assert_str_eq(msg, error_msg_str());
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// command unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_command_ok)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_OK(&cmd, i, f, t, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == OK, "command should be ok, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == 2,  "f should be 2, received %d", f);
  ck_assert_msg(t == 3,  "t should be 3, received %d", t);
  ck_assert_msg(a == 4,  "a should be 4, received %d", a);
}
END_TEST

START_TEST(test_command_nofunds)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_NOFUNDS(&cmd, i, f, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == NOFUNDS, "command should be no funds, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == 2,  "f should be 2, received %d", f);
  ck_assert_msg(t == -1, "t should be -1, received %d", t);
  ck_assert_msg(a == 4,  "a should be 4, received %d", a);
}
END_TEST

START_TEST(test_command_connect)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_CONNECT(&cmd, i);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == CONNECT, "command should be connect, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == -1, "f should be -1, received %d", f);
  ck_assert_msg(t == -1, "t should be -1, received %d", t);
  ck_assert_msg(a == -1, "a should be -1, received %d", a);
}
END_TEST

START_TEST(test_command_exit)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_EXIT(&cmd, i);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == EXIT, "command should be exit, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == -1, "f should be -1, received %d", f);
  ck_assert_msg(t == -1, "t should be -1, received %d", t);
  ck_assert_msg(a == -1, "a should be -1, received %d", a);
}
END_TEST

START_TEST(test_command_deposit)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_DEPOSIT(&cmd, i, t, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == DEPOSIT, "command should be deposit, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == -1, "f should be -1, received %d", f);
  ck_assert_msg(t == 3,  "t should be 3, received %d", t);
  ck_assert_msg(a == 4,  "a should be 4, received %d", a);
}
END_TEST

START_TEST(test_command_withdraw)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_WITHDRAW(&cmd, i, f, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == WITHDRAW, "command should be withdraw, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == 2,  "f should be 2, received %d", f);
  ck_assert_msg(t == -1, "t should be -1, received %d", t);
  ck_assert_msg(a == 4,  "a should be 4, received %d", a);
}
END_TEST

START_TEST(test_command_transfer)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_TRANSFER(&cmd, i, f, t, a);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == TRANSFER, "command should be ok, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == 2,  "f should be 2, received %d", f);
  ck_assert_msg(t == 3,  "t should be 3, received %d", t);
  ck_assert_msg(a == 4,  "a should be 4, received %d", a);
}
END_TEST

START_TEST(test_command_balance)
{
  Command cmd;
  cmd_t c;
  int i = 1, f = 2, t = 3, a = 4;
  MSG_BALANCE(&cmd, i, f);
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == BALANCE, "command should be balance, received %d", c);
  ck_assert_msg(i == 1,  "i should be 1, received %d", i);
  ck_assert_msg(f == 2,  "f should be 2, received %d", f);
  ck_assert_msg(t == -1,  "t should be -1, received %d", t);
  ck_assert_msg(a == -1,  "a should be -1, received %d", a);
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// trace unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_trace_open_bad)
{
  int status = trace_open("test/no_trace_file.trace");
  ck_assert_msg(status == -1, "trace_open should return -1 if the file does not exist");
}
END_TEST

START_TEST(test_trace_open_good)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
}
END_TEST

START_TEST(test_trace_atm_count)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  int atm_cnt = trace_atm_count();
  ck_assert_msg(atm_cnt == 4, "ATM count should be 4, received %d", atm_cnt);
}
END_TEST

START_TEST(test_trace_account_count)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  int account_cnt = trace_account_count();
  ck_assert_msg(account_cnt == 2, "account count should be 2, received %d", account_cnt);
}
END_TEST

START_TEST(test_trace_read_single_cmd)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;
  trace_read_cmd(&cmd);
  cmd_t c;
  int i, f, t, a;
  cmd_unpack(&cmd, &c, &i, &f, &t, &a);
  ck_assert_msg(c == CONNECT, "first command should be CONNECT, received %d", c);
}
END_TEST

START_TEST(test_trace_read_valid_commands)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case CONNECT:
    case EXIT:
    case DEPOSIT:
    case WITHDRAW:
    case TRANSFER:
    case BALANCE:
      break;
    default:
      ck_abort_msg("expected valid command, received %d", c);
    }
  }

  trace_close();
}
END_TEST


START_TEST(test_trace_read_connects)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  int connects  = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case CONNECT:
      connects++;
      break;
    }
  }

  ck_assert_msg(connects == 4, "there should be 4 connect messages, received %d", connects);

  trace_close();
}
END_TEST

START_TEST(test_trace_read_exits)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  int exits     = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case EXIT:
      exits++;
      break;
    }
  }

  ck_assert_msg(exits == 4, "there should be 4 exit messages, received %d", exits);

  trace_close();
}
END_TEST

START_TEST(test_trace_read_deposits)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  int deposits  = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case DEPOSIT:
      deposits++;
      break;
    }
  }

  ck_assert_msg(deposits == 2, "there should be 2 deposit messages, received %d", deposits);

  trace_close();
}
END_TEST

START_TEST(test_trace_read_withdraws)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  int withdraws = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case WITHDRAW:
      withdraws++;
      break;
    }
  }

  ck_assert_msg(withdraws == 3, "there should be 3 withdraw messages, received %d", withdraws);

  trace_close();
}
END_TEST

START_TEST(test_trace_read_transfers)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  int transfers = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case TRANSFER:
      transfers++;
      break;
    }
  }

  ck_assert_msg(transfers == 4, "there should be 4 transfer message, received %d", transfers);

  trace_close();
}
END_TEST

START_TEST(test_trace_read_balances)
{
  int status = trace_open("test/4_2_10.trace");
  ck_assert_msg(status != -1, "trace_open should not return -1 if the file does exists");
  Command cmd;

  int balances  = 0;

  cmd_t c;
  int i, f, t, a;
  while (trace_read_cmd(&cmd) != 0) {
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    switch (c) {
    case BALANCE:
      balances++;
      break;
    }
  }

  ck_assert_msg(balances == 3, "there should be 3 balance messages, received %d", balances);

  trace_close();
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// atm unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_atm_invalid_command_status)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int atm_read   = fd[0];
  int bank_write = fd[1];

  // The atm message buffer.
  Command atmcmd;

  // Create an invalid command (99).
  cmd_pack(&atmcmd, 99, 0, 1, 2, 3);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ck_assert_msg(status == ERR_UNKNOWN_CMD,
		"atm should return ERR_UNKNOWN_CMD, received %d",
		status);

  close(atm_read);
  close(bank_write);
}
END_TEST

START_TEST(test_atm_unknown_atm)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int atm_read   = fd[0];
  int bank_write = fd[1];

  // The atm message buffer.
  Command atmcmd;

  // Create a connect message for ATM 0.
  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm with the incorrect ATM id.
  int status = atm(bank_write, atm_read, 1, &atmcmd);

  ck_assert_msg(status == ERR_UNKNOWN_ATM,
		"atm should return ERR_UNKNOWN_ATM, received %d",
		status);

  close(atm_read);
  close(bank_write);
}
END_TEST

START_TEST(test_atm_invalid_pipe_write)
{
  // Invalid pipe descriptors
  int atm_read   = -1;
  int bank_write = -1;

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ck_assert_msg(status == ERR_PIPE_WRITE_ERR,
		"atm should return ERR_PIPE_WRITE_ERR, received %d",
		status);
}
END_TEST

START_TEST(test_atm_invalid_pipe_read)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int bank_write = fd[1];
  // But, we invalidate the read pipe.
  int atm_read   = -1;

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ck_assert_msg(status == ERR_PIPE_READ_ERR,
		"atm should return ERR_PIPE_READ_ERR, received %d",
		status);
}
END_TEST

START_TEST(test_atm_invalid_pipe_write_closed)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int bank_write = fd[1];
  int atm_read   = fd[0];

  // But, we close the write end of the pipe.
  close(bank_write);

  // Closing the write end of the pipe will cause a write to that pipe
  // to return 0 indicating EOF. The atm function must handle this
  // case. It will return a status indicating ERR_PIPE_WRITE_ERR.

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &atmcmd);

  ck_assert_msg(status == ERR_PIPE_WRITE_ERR,
		"atm should return ERR_PIPE_WRITE_ERR, received %d",
		status);
}
END_TEST

START_TEST(test_atm_invalid_pipe_read_signal)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int fd[2];
  pipe(fd);
  int bank_write = fd[1];
  int atm_read   = fd[0];

  // But, we close the read pipe.
  close(atm_read);

  // This will create a "broken" pipe. If the ATM tries to write to a
  // broken pipe it will send a SIGPIPE signal to the process. This is
  // checked in the test case below.

  // The atm message buffer.
  Command atmcmd;

  MSG_CONNECT(&atmcmd, 0);

  // Send it to the atm.
  atm(bank_write, atm_read, 0, &atmcmd);
}
END_TEST

START_TEST(test_atm_connect_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read  = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read   = atmfd[0];
  int atm_write  = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_CONNECT(&cmd, 0);

  if (fork() == 0) {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
  }
  else {
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have sent data to the bank");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == CONNECT, "received message should be connect, got %d", c);
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}
END_TEST

START_TEST(test_atm_exit_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read  = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read   = atmfd[0];
  int atm_write  = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_EXIT(&cmd, 0);

  if (fork() == 0) {
  // Send it to the atm.
  int status = atm(bank_write, atm_read, 0, &cmd);
  ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have sent data to the bank");
  
    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == EXIT, "received message should be exit, got %d", c);
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}
END_TEST

START_TEST(test_atm_withdraw_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read  = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read   = atmfd[0];
  int atm_write  = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_WITHDRAW(&cmd, 0, 1, 200);

  if (fork() == 0) {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have sent data to the bank");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == WITHDRAW, "received message should be withdraw, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 1);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, 200);
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }

  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}
END_TEST

START_TEST(test_atm_transfer_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read  = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read   = atmfd[0];
  int atm_write  = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_TRANSFER(&cmd, 0, 1, 2, 200);

  if (fork() == 0) {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have sent data to the bank");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == TRANSFER, "received message should be transfer, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 1);
    ck_assert_int_eq(t, 2);
    ck_assert_int_eq(a, 200);
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}
END_TEST

START_TEST(test_atm_balance_success)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read  = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read   = atmfd[0];
  int atm_write  = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_BALANCE(&cmd, 0, 1);

  if (fork() == 0) {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have sent data to the bank");
    
    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == BALANCE, "received message should be balance, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 1);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, -1);
    MSG_OK(&cmd, i, f, t, a);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}
END_TEST

START_TEST(test_atm_balance_no_funds)
{
  // The file descriptors used for writing to the bank process and
  // reading in from the atm process.
  int bankfd[2];
  pipe(bankfd);
  int atmfd[2];
  pipe(atmfd);

  int bank_read  = bankfd[0];
  int bank_write = bankfd[1];
  int atm_read   = atmfd[0];
  int atm_write  = atmfd[1];

  // The command buffer.
  Command cmd;

  MSG_BALANCE(&cmd, 0, 1);

  if (fork() == 0) {
    // Send it to the atm.
    int status = atm(bank_write, atm_read, 0, &cmd);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(bank_read, F_GETFL, 0);
    fcntl(bank_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(bank_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have sent data to the bank");
    
    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == BALANCE, "received message should be balance, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 1);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, -1);
    MSG_NOFUNDS(&cmd, 0, f, 10);
    write(atm_write, &cmd, MESSAGE_SIZE);
  }
  // Close file descriptors.
  close(bankfd[0]);
  close(bankfd[1]);
  close(atmfd[0]);
  close(atmfd[1]);
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// bank unit tests
//////////////////////////////////////////////////////////////////////

START_TEST(test_bank_connect)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read     = atmfd[0];
  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    MSG_CONNECT(&cmd, 0);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "BANK should have received data from the ATM");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == OK, "received message should be ok, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, -1);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, -1);
  }
}
END_TEST

START_TEST(test_bank_exit)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read     = atmfd[0];
  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    MSG_EXIT(&cmd, 0);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
    ck_assert_msg(atm_cnt == 0, "the ATM count should now be 0, received %d", atm_cnt);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "BANK should have received data from the ATM");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == OK, "received message should be ok, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, -1);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, -1);
  }
}
END_TEST

START_TEST(test_bank_deposit)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read     = atmfd[0];
  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    MSG_DEPOSIT(&cmd, 0, 0, 200);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
    int* accounts = get_accounts();
    ck_assert_msg(accounts[0] == 200, "account should have 200, found %d", accounts[0]);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "BANK should have received data from the ATM");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == OK, "received message should be ok, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, -1);
    ck_assert_int_eq(t, 0);
    ck_assert_int_eq(a, 200);
  }
}
END_TEST

START_TEST(test_bank_withdraw)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read     = atmfd[0];
  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    int* accounts = get_accounts();
    accounts[0] = 200;
    MSG_WITHDRAW(&cmd, 0, 0, 25);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
    ck_assert_msg(accounts[0] == 175, "account should have 175, found %d", accounts[0]);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "BANK should have received data from the ATM");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == OK, "received message should be ok, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 0);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, 25);
  }
}
END_TEST

START_TEST(test_bank_transfer)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read     = atmfd[0];
  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    int* accounts = get_accounts();
    accounts[0] = 200;
    MSG_TRANSFER(&cmd, 0, 0, 0, 25);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
    ck_assert_msg(accounts[0] == 200, "account should have 200, found %d", accounts[0]);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "BANK should have received data from the ATM");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == OK, "received message should be ok, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 0);
    ck_assert_int_eq(t, 0);
    ck_assert_int_eq(a, 25);
  }
}
END_TEST

START_TEST(test_bank_balance)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_read     = atmfd[0];
  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    int* accounts = get_accounts();
    accounts[0] = 200;
    MSG_BALANCE(&cmd, 0, 0);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be SUCCESS, received %d", status);
    ck_assert_msg(accounts[0] == 200, "account should have 200, found %d", accounts[0]);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atm_read, F_GETFL, 0);
    fcntl(atm_read, F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atm_read, &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "BANK should have received data from the ATM");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == OK, "received message should be ok, got %d", c);
    ck_assert_int_eq(i, 0);
    ck_assert_int_eq(f, 0);
    ck_assert_int_eq(t, -1);
    ck_assert_int_eq(a, 200);
  }
}
END_TEST

START_TEST(test_bank_balance_bad_account)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  if (fork() == 0) {
    bank_open(1, 1);
    int* accounts = get_accounts();
    accounts[0] = 200;
    MSG_BALANCE(&cmd, 0, 1);
    int atm_cnt = 1;
    int status = bank(atm_write, &cmd, &atm_cnt);
    ck_assert_msg(status == SUCCESS, "status should be ERR_UNKNOWN_ACCOUNT, received %d", status);
    ck_assert_msg(accounts[0] == 200, "account should have 200, found %d", accounts[0]);
    bank_close();
  }
  else {
    // Need to set the read end of pipe to non-blocking otherwise
    // students without an implementation will not be able to run
    // tests. 
    int flags = fcntl(atmfd[0], F_GETFL, 0);
    fcntl(atmfd[0], F_SETFL, flags | O_NONBLOCK);

    sleep(1);
    int status = read(atmfd[0], &cmd, MESSAGE_SIZE);
    ck_assert_msg(status != -1, "ATM should have received data from the BANK");

    cmd_t c;
    int i, f, t, a;
    cmd_unpack(&cmd, &c, &i, &f, &t, &a);
    ck_assert_msg(c == ACCUNKN, "bank should have sent ACCUNKN, received %d", c);
  }
}
END_TEST

START_TEST(test_bank_balance_bad_atm)
{
  int atmfd[2];
  pipe(atmfd);

  int atm_write[1] = { atmfd[1] };

  // The command buffer.
  Command cmd;

  bank_open(1, 1);
  int* accounts = get_accounts();
  accounts[0] = 200;
  MSG_BALANCE(&cmd, 1, 0);
  int atm_cnt = 1;
  int status = bank(atm_write, &cmd, &atm_cnt);
  ck_assert_msg(status == ERR_UNKNOWN_ATM, "status should be ERR_UNKNOWN_ATM, received %d", status);
  ck_assert_msg(accounts[0] == 200, "account should have 200, found %d", accounts[0]);
  bank_close();
}
END_TEST

//////////////////////////////////////////////////////////////////////
///////////// Test Suite
//////////////////////////////////////////////////////////////////////

Suite* tester_suite() {
  // Create the test suite:
  Suite* s = suite_create("Public Assignment Tests");

  /**** BEGIN UNIT TESTS ****/
  TCase* tc_inc = tcase_create("Public Tests");
  // TODO: fill in for p
  tcase_add_test(tc_inc, test_error_msg_SUCCESS);
  tcase_add_test(tc_inc, test_error_msg_ERR_UNKNOWN_CMD);
  tcase_add_test(tc_inc, test_error_msg_ERR_PIPE_READ_ERR);
  tcase_add_test(tc_inc, test_error_msg_ERR_PIPE_WRITE_ERR);
  tcase_add_test(tc_inc, test_error_msg_ERR_UNKNOWN_ACCOUNT);
  tcase_add_test(tc_inc, test_error_msg_ERR_UNKNOWN_ATM);
  tcase_add_test(tc_inc, test_error_msg_ERR_BAD_TRACE_FILE);

  tcase_add_test(tc_inc, test_command_ok);
  tcase_add_test(tc_inc, test_command_nofunds);
  tcase_add_test(tc_inc, test_command_connect);
  tcase_add_test(tc_inc, test_command_exit);
  tcase_add_test(tc_inc, test_command_deposit);
  tcase_add_test(tc_inc, test_command_withdraw);
  tcase_add_test(tc_inc, test_command_transfer);
  tcase_add_test(tc_inc, test_command_balance);

  tcase_add_test(tc_inc, test_trace_open_bad);
  tcase_add_test(tc_inc, test_trace_open_good);
  tcase_add_test(tc_inc, test_trace_atm_count);
  tcase_add_test(tc_inc, test_trace_account_count);
  tcase_add_test(tc_inc, test_trace_read_valid_commands);
  tcase_add_test(tc_inc, test_trace_read_single_cmd);
  tcase_add_test(tc_inc, test_trace_read_connects);
  tcase_add_test(tc_inc, test_trace_read_exits);
  tcase_add_test(tc_inc, test_trace_read_deposits);
  tcase_add_test(tc_inc, test_trace_read_withdraws);
  tcase_add_test(tc_inc, test_trace_read_transfers);
  tcase_add_test(tc_inc, test_trace_read_balances);

  tcase_add_test(tc_inc, test_atm_invalid_command_status);
  tcase_add_test(tc_inc, test_atm_invalid_pipe_write);
  tcase_add_test(tc_inc, test_atm_invalid_pipe_read);
  tcase_add_test(tc_inc, test_atm_invalid_pipe_write_closed);
  tcase_add_test_raise_signal(tc_inc, test_atm_invalid_pipe_read_signal, 13);
  tcase_add_test(tc_inc, test_atm_unknown_atm);
  tcase_add_test(tc_inc, test_atm_connect_success);
  tcase_add_test(tc_inc, test_atm_exit_success);
  tcase_add_test(tc_inc, test_atm_withdraw_success);
  tcase_add_test(tc_inc, test_atm_transfer_success);
  tcase_add_test(tc_inc, test_atm_balance_success);
  tcase_add_test(tc_inc, test_atm_balance_no_funds);

  tcase_add_test(tc_inc, test_bank_connect);
  tcase_add_test(tc_inc, test_bank_exit);
  tcase_add_test(tc_inc, test_bank_deposit);
  tcase_add_test(tc_inc, test_bank_withdraw);
  tcase_add_test(tc_inc, test_bank_transfer);
  tcase_add_test(tc_inc, test_bank_balance);
  tcase_add_test(tc_inc, test_bank_balance_bad_account);
  tcase_add_test(tc_inc, test_bank_balance_bad_atm);

  suite_add_tcase(s, tc_inc);
  /**** END UNIT TESTS   ****/

  // Return the suite:
  return s;
}

//////////////////////////////////////////////////////////////////////
///////////// Test Main Routine
//////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
  int number_failed;

  printf("############################################################\n");
  printf("# NOTE: THESE TESTS TAKE SOME TIME TO COMPLETE. BE PATIENT #\n");
  printf("############################################################\n");
  
  Suite* s = tester_suite();
  SRunner* sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
