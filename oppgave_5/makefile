INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)
TFLAG = -lpthread

OBJDIR = obj

_DEPS = oppgave_5.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = oppgave_5.o client.o server.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)


oppgave_5: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS) $(TFLAG)

.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*