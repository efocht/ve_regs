/*
 * Little demo for displaying registers of a VE program while it is running.
 *
 * Prerequisite: veosinfo-devel libved-devel
 *
 * Compile:
 * gcc -o ve_regs ve_regs.c -I/opt/nec/ve/veos/include -L/opt/nec/ve/veos/lib64 -lveosinfo
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include "ve_regs_helper.h"

void print_regnames()
{
	int i;
	for (i = 0; i < NUM_USER_REGS; i++) {
		printf("%s ", usr_reg_name[i]);
	}
	printf("\n");
}

void help(char *cmd)
{
	fprintf(stderr, "Usage:\n");
	fprintf(stderr, "\t%s [-h|--help] [-l|--list-regs] [-p|--pid <PID>]"
		" [-f|--fmt U|L|X|p] <regid> ...\n\n", cmd);
	fprintf(stderr, "Options:\n");
	fprintf(stderr, "\t-l|--list-regs   : list the symbolic register IDs.\n");
	fprintf(stderr, "\t-p|--pid <pid>   : specify VE process ID.\n");
	fprintf(stderr, "\t-f|--fmt U|L|X|p : print format of register values: unsigned, signed or hex.\n");
	fprintf(stderr, "\t<regid> ...      : up to 64 symbolic register names like USRCC, PMC01, SR12...\n");
	fprintf(stderr, "\t                   list them with the -l option.\n");
}

int main(int argc, char *argv[])
{
	int i = 0, rc = -1, vefd = -1;
	int pid = 0;
	int regid[VE_MAX_REGVALS];
	int numregs = 0;
	uint64_t regval[VE_MAX_REGVALS];
	int node_id = 0;
	int s, option_index;
	char *cmd = strdup(argv[0]);
	char fmt = 'U';

	static struct option long_options[] = {
		{"pid", required_argument, NULL, 'p'},
		{"fmt", required_argument, NULL, 'f'},
		{"list-regs", no_argument, NULL, 'l'},
		{"help", no_argument, NULL, 'h'},
		{0, 0, 0, 0}
	};
	while (1) {
		s = getopt_long(argc, argv, "+hp:lf:", long_options,
				&option_index);
		if (s == -1)
			break;
		switch (s) {
		case 'p':
			pid = atoi(optarg);
			break;
		case 'f':
			fmt = optarg[0];
			if (fmt != 'X' && fmt != 'L' && fmt != 'U' && fmt != 'p') {
				fprintf(stderr, "Unknown --fmt argument. Must be one of"
					" X, L, U, p for, respectively, hexadecimal, "
					"signed long, unsigned long or parseable representation.\n");
				help(cmd);
				return 1;
			}
			break;
		case 'l':
			print_regnames();
			return 0;
			break;
		case 'h':
			help(cmd);
			return 0;
			break;
		default:
			help(cmd);
			return 1;
			break;
		}
	}
	if (!pid) {
		fprintf(stderr, "No PID passed as argument.\n");
		help(cmd);
		return 1;
	}
	node_id = nodeid_of_pid(pid);
	if (node_id == -1) {
		fprintf(stderr, "VE Node for PID %d not found!\n", pid);
		return 2;
	}
	if (argc > 1) {
		if (argc - 1 > VE_MAX_REGVALS) {
			fprintf(stderr, "Limiting number of transfered register values to %d\n",
				VE_MAX_REGVALS);
			argc = VE_MAX_REGVALS + 1;
		}
		for (i = 1; i < argc; i++) {
			int idx = regname_offs(argv[i]);
			if (idx >= 0)
				regid[numregs++] = idx;
		}
		/*
		 * Retrieve numregs register values for the process pid on VE nodeid.
		 * The register offsets are listed in the array regid.
		 * Results are returned in the array regval.
		 */
		rc = ve_get_regvals(node_id, pid, numregs, regid, regval);
		if (rc != 0) {
			fprintf(stderr, "ve_get_regvals failed with rc=%d\n", rc);
			return 1;
		}
		for (i = 0; i < numregs; i++) {
			if (fmt == 'X')
				printf("%s=%p\n", usr_reg_name[regid[i]], (void *)regval[i]);
			else if (fmt == 'U')
				printf("%s=%lu\n", usr_reg_name[regid[i]], regval[i]);
			else if (fmt == 'L')
				printf("%s=%ld\n", usr_reg_name[regid[i]], *(long *)&regval[i]);
			else if (fmt == 'p')
				printf("%lu ", regval[i]);
		}
		if (fmt == 'p')
			printf("\n");
	}
	return 0;
}
