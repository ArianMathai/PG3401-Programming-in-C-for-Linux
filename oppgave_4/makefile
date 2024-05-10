INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)
TFLAG = -lpthread

OBJDIR = obj

_DEPS = sha1.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = eksamen_v24_oppgave4.o sha1.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)


oppgave_4: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS) $(TFLAG)

.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*