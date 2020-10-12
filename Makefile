#
#	DKU Operating System Lab
#	    Lab2 (BST Synchronization)
#	    Student id : 
#	    Student name : 
#
#   Makfeile :
#       - Makefile for lab2 compilation.
#

CC = gcc
INC = -I${CURDIR}/include/
CFLAGS = -g $(INC)
LDFLAGS = -lpthread

OBJS_LAB2 = lab2_bst.o lab2_bst_test.o include/lab2_timeval.o 
OBJS_RW = readers-writers.o rwstride.o include/rwlock.o


SRCS = $(OBJS_LAB2:.o=.c) $(OBJS_RW:.o=.c) 

TARGET_LAB2 = lab2_bst

.SUFFIXES : .c .o

.c.o:
	@echo "Compilingi lab2 BST synchronization $< ..."
	$(CC) -c $(CFLAGS) -o $@ $<

$(TARGET_LAB2) : $(OBJS_LAB2) $(OBJS_RW)
	$(CC) -o $(TARGET_LAB2) $(OBJS_RW) $(OBJS_LAB2)  $(LDFLAGS)


all : $(TARGET_LAB2) $(TARGET_BONUS) $(TARGET_EXAMPLE)

dep : 
	gccmaedep $(INC) $(SRCS)

clean :
	@echo "Cleaning lab2 BST synchronization $< ..."
	rm -rf $(OBJS_LAB2) $(TARGET_LAB2) $(OBJS_RW) 

new :
	$(MAKE) clean
	$(MAKE)
