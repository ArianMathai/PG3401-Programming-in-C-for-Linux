INCLDIR = ./include
CC = gcc
CFLAGS = -O2
CFLAGS += -I$(INCLDIR)

OBJDIR = obj

_DEPS = oppgave_2.h eksamen_v24_oppgave2_all_lower.h eksamen_v24_oppgave2_all_upper.h eksamen_v24_oppgave2_anagram.h eksamen_v24_oppgave2_disjoint.h eksamen_v24_oppgave2_heterogram.h eksamen_v24_oppgave2_palindrom.h
DEPS = $(patsubst %,$(INCLDIR)/%,$(_DEPS))

_OBJS = oppgave_2.o eksamen_v24_oppgave2_all_lower.o eksamen_v24_oppgave2_all_upper.o eksamen_v24_oppgave2_anagram.o eksamen_v24_oppgave2_disjoint.o eksamen_v24_oppgave2_heterogram.o eksamen_v24_oppgave2_palindrom.o
OBJS = $(patsubst %,$(OBJDIR)/%,$(_OBJS))

$(OBJDIR)/%.o: %.c $(DEPS)
		$(CC) -c -o $@ $< $(CFLAGS)


oppgave_2: $(OBJS)
		$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
		rm -f $(OBJDIR)/*.o *~ core $(INCLDIR)/*