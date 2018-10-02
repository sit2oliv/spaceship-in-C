.PHONY: all archive clean distclean

NAME ?= $(shell basename $(shell pwd))
LDLIBS ?= -lm -lncursesw
CFLAGS ?= -O3 -march=native -g3 -ggdb
override CFLAGS += -std=gnu11 -pedantic -pedantic-errors \
		-Wall -Wextra \
		-Wdouble-promotion -Wformat=2 -Winit-self -Wswitch-default \
		-Wswitch-enum -Wunused-parameter -Wuninitialized -Wfloat-equal \
		-Wshadow -Wundef -Wbad-function-cast -Wcast-qual -Wcast-align \
		-Wwrite-strings -Wconversion -Wstrict-prototypes \
		-Wold-style-definition -Wmissing-prototypes -Wmissing-declarations \
		-Wredundant-decls -Wnested-externs
# D'autres warnings intéressants (en général, certains sont inutiles dans ce
# cas particulier) mais pas encore reconnus par la version de GCC disponible
# sur une Ubuntu 14.04... :
#
# -Wmissing-include-dirs -Wnull-dereference -Wswitch-bool -Wduplicated-cond
# -Wdate-time

EXEC = spaceship-infinity
all: $(EXEC)
spaceship-infinity: spaceship-infinity.o options.o game.o column_list.o terrain.o ui.o column.o point_list.o
	$(CC) $(LDFLAGS) $^ -o $@ $(LOADLIBES) $(LDLIBS)

# Archive
archive:
	tar -czf $(NAME).tar.gz --transform="s,^,$(NAME)/," *.c *.h Makefile

# Nettoyage
clean:
	$(RM) -r $(EXEC) *.o
distclean: clean
	$(RM) *.tar.gz

# Dépendances avec les en-têtes
spaceship-infinity.o: spaceship-infinity.c game.h point.h point_list.h \
	terrain.h column.h cell.h column_list.h options.h ui.h
ui.o: ui.c ui.h game.h point.h point_list.h terrain.h column.h cell.h \
	column_list.h options.h
game.o: game.c game.h point.h point_list.h terrain.h column.h cell.h \
	column_list.h options.h
terrain.o: terrain.c terrain.h point.h column.h cell.h column_list.h
column_list.o: column_list.c column_list.h column.h cell.h
column.o: column.c column.h cell.h
options.o: options.c options.h
point_list.o: point_list.c point_list.h point.h
