#ifndef _VE_REGS_HELPER_H_
#define _VE_REGS_HELPER_H_

#include <libved.h>
#include <veosinfo/veosinfo.h>

#define NUM_USER_REGS (SR63 + 1)
/*
 * Array of register name strings.
 * The VE register offsets are defined in "enum usr_reg" in libved.h
 * which belongs to the package libved-devel.
 */
const char* usr_reg_name[NUM_USER_REGS];

/*
 * Find first VE node on which the PID is running.
 */
int nodeid_of_pid(pid_t pid);

/*
 * Return offset corresponding to a register name.
 */
int regname_offs(const char *name);

#endif /* _VE_REGS_HELPER_H_ */
