#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include "ve_regs_helper.h"

/*
 * Array of register name strings, named according to the
 * enum usr_reg
 * located in libved.h
 */
const char* usr_reg_name[] = {
	"USRCC",
	"PMC00", "PMC01", "PMC02", "PMC03", "PMC04", "PMC05", "PMC06",
	"PMC07", "PMC08", "PMC09", "PMC10", "PMC11", "PMC12", "PMC13",
	"PMC14", "PMC15",
	"PSW", "EXS", "IC", "ICE", "VIXR", "VL", "SAR",
	"PMMR", "PMCR00", "PMCR01", "PMCR02", "PMCR03", "PMCR04",
	"SR00", "SR01", "SR02", "SR03", "SR04", "SR05", "SR06", "SR07",
	"SR08", "SR09", "SR10", "SR11", "SR12", "SR13", "SR14", "SR15",
	"SR16", "SR17", "SR18", "SR19", "SR20", "SR21", "SR22", "SR23",
	"SR24", "SR25", "SR26", "SR27", "SR28", "SR29", "SR30", "SR31",
	"SR32", "SR33", "SR34", "SR35",	"SR36", "SR37", "SR38", "SR39",
	"SR40", "SR41", "SR42", "SR43", "SR44", "SR45", "SR46", "SR47",
	"SR48", "SR49", "SR50", "SR51", "SR52", "SR53", "SR54", "SR55",
	"SR56", "SR57", "SR58", "SR59",	"SR60", "SR61", "SR62", "SR63"
};

/*
 * Return offset corresponding to a register name.
 */
int regname_offs(const char *name)
{
	int i, offs = -1;
	for (i = 0; i < NUM_USER_REGS; i++) {
		if (strcmp(name, usr_reg_name[i]) == 0) {
			offs = i;
			break;
		}
	}
	return offs;	
}

/*
 * Find first VE node on which the PID is running.
 */
int nodeid_of_pid(pid_t pid)
{
	int i, nodeid;
	struct ve_nodeinfo ni;

	if (ve_node_info(&ni) != 0) {
		fprintf(stderr, "ve_node_info failed!\n");
		return 0;
	}
	// find pid in one of the VE nodes...
	for (i = 0; i < ni.total_node_count; i++) {
		if (ni.status[i])
			continue;
		nodeid = ni.nodeid[i];
		if (!ve_check_pid(nodeid, pid))
			return nodeid;
	}
	return -1;
}
