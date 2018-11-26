#ifndef __COMMAND_H
#define __COMMAND_H

// The byte type represents the rest of the bytes.
typedef unsigned char byte;

// A command is represented as a sequence of 17 bytes:
//
//   ciiiiffffttttaaaa
//
// Where the byte c is the command, the integer iiii is the id, the
// integer ffff is the from account, the integer tttt is the to
// account, and the integer aaaa is the monetary amount.

typedef struct command {
  byte cmd [1];
  byte id  [4];
  byte from[4];
  byte to  [4];
  byte amt [4];
} Command;

// The size of a command message.
#define MESSAGE_SIZE (sizeof(Command))

// The different type of commands that can be sent/received.
#define OK       0
#define CONNECT  1
#define EXIT     2
#define DEPOSIT  3
#define WITHDRAW 4
#define TRANSFER 5
#define BALANCE  6
#define NOFUNDS  7
#define ATMUNKN  8
#define ACCUNKN  9

// Macros for constructing command messages. You should use these
// macros to construct command messages in your ATM and bank
// implementation. You could use the `cmd_pack` function directly,
// however, this is a nicer and more obvious shorthand.
#define MSG_OK(cmd,id,f,t,a)       cmd_pack((cmd),OK,(id),(f),(t),(a))
#define MSG_NOFUNDS(cmd,id,f,a)    cmd_pack((cmd),NOFUNDS,(id),(f),-1,(a))
#define MSG_CONNECT(cmd,id)        cmd_pack((cmd),CONNECT,(id),-1,-1,-1)
#define MSG_EXIT(cmd,id)           cmd_pack((cmd),EXIT,(id),-1,-1,-1)
#define MSG_DEPOSIT(cmd,id,t,a)    cmd_pack((cmd),DEPOSIT,(id),-1,(t),(a))
#define MSG_WITHDRAW(cmd,id,f,a)   cmd_pack((cmd),WITHDRAW,(id),(f),-1,(a))
#define MSG_TRANSFER(cmd,id,f,t,a) cmd_pack((cmd),TRANSFER,(id),(f),(t),(a))
#define MSG_BALANCE(cmd,id,f)      cmd_pack((cmd),BALANCE,(id),(f),-1,-1)
#define MSG_ATMUNKN(cmd,id)        cmd_pack((cmd),ATMUNKN,(id),-1,-1,-1)
#define MSG_ACCUNKN(cmd,id,t)      cmd_pack((cmd),ACCUNKN,(id),-1,(t),-1)

// A cmd_t type represents the command byte in the command message.
typedef unsigned char cmd_t;

// `cmd_pack` is used to "pack" each part of a command into the
// provided command buffer. The parameters are:
//
// cmd    - the command buffer
// c      - the command type
// id     - the atm/bank id
// from   - the from account (if any)
// to     - the to account (if any)
// amt    - the amount of the transaction (if any)
void cmd_pack (Command *cmd, cmd_t c, int id, int from, int to, int amt);

// `cmd_unpack` is used to "unpack" a previously "packed" command
// buffer into the corresponds parts (the rest of the parameters).
//
// cmd    - the command buffer
// c      - the command type
// id     - the atm/bank id
// from   - the from account (if any)
// to     - the to account (if any)
// amt    - the amount of the transaction (if any)
void cmd_unpack (Command *cmd, cmd_t *c, int *id, int *from, int *to, int *amt);

// `cmd_dump` will dump the command parts to standard output. The
// `msg` and `id` are included in the output. This function will dump
// the command only if the `BANKSIM_DEBUG` environment variable has
// been set. To do this in bash you would:
//
// $ export BANKSIM_DEBUG=1
//
// To only set the environment variable during a program run you would
// do this:
//
// $ BANKSIM_DEBUG=1 ./banksim 10_10_100.trace
void cmd_dump (char *msg, int id, Command *cmd);

#endif
