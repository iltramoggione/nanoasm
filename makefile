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
DEP     :=./dep
INCLUDE :=./include
SRC     :=./src
DEPFLAGS:= -MT $@ -MMD -MP -MF $(OBJ)/$*.d
SRCS    :=$(wildcard $(SRC)/*.c)
OBJS    :=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))
DEPS    :=$(patsubst $(SRC)/%.c,$(DEP)/%.d,$(SRCS))
EXE     :=$(BIN)/nanoasm$(EXEEXT)
LOG     :=log.txt
CFLAGS  :=-Wall -Wno-comment -Werror -lpthread -std=c11
LDLIBS  :=

.PHONY: build run log err tee cleanobj cleanbin cleandep cleannotbin clean all allrun alllog allerr alltee

build: $(EXE)

$(EXE): $(OBJS) | $(BIN)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ)/%.o: $(SRC)/%.c | $(OBJ) $(DEP)
	$(CC) $(CFLAGS) -MT $@ -MMD -MP -MF $(DEP)/$*.d -c $< -o $@

$(BIN) $(OBJ) $(DEP):
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

cleandep:
	$(RMDIR) $(DEP)

cleannotbin: cleanobj cleandep

clean: cleannotbin cleanbin

all: clean build cleanobj

allrun: clean build cleanobj run

alllog: clean build cleanobj log

allerr: clean build cleanobj err

alltee: clean build cleanobj tee

include $(wildcard $(DEPS))