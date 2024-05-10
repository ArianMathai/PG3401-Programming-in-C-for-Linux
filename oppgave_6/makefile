INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = oppgave_6.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = oppgave_6.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)


oppgave_6: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*