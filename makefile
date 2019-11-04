ifdef OS
#Windows
EXEEXT  :=.exe
else
#Unix
EXEEXT  :=
endif
MKDIR   :=mkdir
RMDIR   :=rm -rf
CC      :=gcc
BIN     :=./bin
OBJ     :=./obj
INCLUDE :=./include
SRC     :=./src
SRCS    :=$(wildcard $(SRC)/*.c)
OBJS    :=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
EXE     :=$(BIN)/nanoasm$(EXEEXT)
LOG     :=log.txt
CFLAGS  :=-Wall -Wno-comment -Werror -lpthread -std=c11
LDLIBS  :=

.PHONY: build run log err tee cleanobj cleanbin clean all allrun alllog allerr alltee

build: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN) $(OBJ):
	$(MKDIR) $@

run: $(EXE)
	$<

log: $(EXE)
	$< 2>&1 > $(LOG)

err: $(EXE)
	$< 2> $(LOG)

tee: $(EXE)
	$< 2>&1 | tee $(LOG)

cleanobj:
	$(RMDIR) $(OBJ)

cleanbin:
	$(RMDIR) $(BIN)

clean: cleanobj cleanbin

all: clean build cleanobj

allrun: clean build cleanobj run

alllog: clean build cleanobj log

allerr: clean build cleanobj err

alltee: clean build cleanobj tee