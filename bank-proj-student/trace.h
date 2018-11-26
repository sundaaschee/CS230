#ifndef __TRACE_H
#define __TRACE_H

#include "command.h"

// `trace_open` opens a trace file for processing. It returns -1 if
// there was a problem.
int trace_open (char *path);

// `trace_close` closes a trace file.
void trace_close ();

// `trace_read_cmd` reads the next command from the trace file into
// the command buffer `cmd`. It returns the same return values as the
// `read` system call (0 when the trace is done).
int trace_read_cmd (Command *cmd);

// `trace_atm_count` returns the number of ATMs the trace was
// generated for.
int trace_atm_count ();

// `trace_account_count` returns the number of bank accounts the trace
// was generated for.
int trace_account_count ();

#endif
