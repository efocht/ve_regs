#
# trivial makefile for ve_regs
#

CFLAGS = -I/opt/nec/ve/veos/include
LDFLAGS = -L/opt/nec/ve/veos/lib64 -lveosinfo

all: ve_regs

ve_regs_helper.o: ve_regs_helper.c ve_regs_helper.h
	$(CC) -c $< $(CFLAGS) -o $@

ve_regs.o: ve_regs.c ve_regs_helper.h
	$(CC) -c $< $(CFLAGS) -o $@

ve_regs: ve_regs.o ve_regs_helper.o
	$(CC) $^ -o $@ $(LDFLAGS)

clean:
	rm ve_regs *.o
