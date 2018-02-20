TARGET=$(BIN)/edoc
ARGUMENTS=-e -f test/container

BIN=bin
SBIN=sbin
SRC=src
OBJ=obj
INCLUDE=include
LIB=lib
EXTLIB=extlib
SCRIPT=script
DOC=doc

CC=gcc
CFLAGS=-std=c99 -g -Wall -c -I$(INCLUDE) -D_GNU_SOURCE -fpic -DPIC -Os -fPIC
LFLAGS=-L$(LIB)

MKDIR=mkdir -p
RM=rm -f
RMDIR=rm -rf
CP=cp -f
CPDIR=cp -rf
MV=mv -f
ECHO=@echo
INSTALL=apt-get install -y

#MODULES_=archiver dearchiver compressor decompressor edoc map readbuffer writebuffer encoder decoder sbox pbox spbox
MODULES_=edoc readbuffer writebuffer archiver
MODULES=$(addsuffix .o, $(addprefix $(OBJ)/, $(MODULES_)))

.PHONY: help installdep version major minor build clean debug leakcheck profile run trac

all: edoc

help:
	$(ECHO) "edoc"
	$(ECHO) "doc"
	$(ECHO) "major"
	$(ECHO) "minor"
	$(ECHO) "commit"
	$(ECHO) "version"
	$(ECHO) "clean"
	$(ECHO) "installdep"
	$(ECHO) "install"
	$(ECHO) "debug"
	$(ECHO) "leakcheck"
	$(ECHO) "profile"

installdep:
	$(INSTALL) gcc
	$(INSTALL) doxygen
	$(INSTALL) valgrind
	$(INSTALL) kcachegrind
	$(INSTALL) git
	$(INSTALL) firefox
	$(INSTALL) binutils
	$(INSTALL) tar
	$(INSTALL) python
	$(INSTALL) ddd
	$(INSTALL) gdb

$(LIB)/libedoc.a: $(MODULES)
	$(MKDIR) $(LIB)
	ar r $@ $^

$(BIN)/edoc: $(OBJ)/edoc.o $(LIB)/libedoc.a
	$(MKDIR) $(BIN)
	$(CC) -o $@ $< $(LFLAGS) -ledoc
	make build

major:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h --major
	git add $(SRC)/*
	git add $(INCLUDE)/*
	git commit -a
	VERSION=$$(python $(SCRIPT)/v.py $(INCLUDE)/version.h);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "major build";\
	trac-admin ./.trac version add $$VERSION
	git checkout development
	git merge topic
	git checkout release
	git merge development
	git checkout topic

minor:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h --minor
	git add $(SRC)/*
	git add $(INCLUDE)/*
	git commit -a
	VERSION=$$(python $(SCRIPT)/v.py $(INCLUDE)/version.h);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "minor build";\
	trac-admin ./.trac version add $$VERSION
	git checkout development
	git merge topic
	git checkout topic

build:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h --build
	git add $(SRC)/*
	git add $(INCLUDE)/*
	git commit -am "normal build"
	VERSION=$$(python $(SCRIPT)/v.py $(INCLUDE)/version.h);\
	VERSION="v"$$VERSION;\
	git tag -a $$VERSION -m "normal build";\
	trac-admin ./.trac version add $$VERSION

commit:
	git add $(SRC)
	git add $(INCLUDE)
	git commit -a

version:
	python $(SCRIPT)/v.py $(INCLUDE)/version.h

clean:
	$(RMDIR) $(BIN)
	$(RMDIR) $(OBJ)
	$(RMDIR) $(LIB)
	$(RMDIR) $(DOC)
	$(RM) cachegrind.*
	$(RM) core
	$(RM) vgcore.*
	$(RM) $(SRC)/.*.swp
	$(RM) $(INCLUDE)/.*.swp
	$(RM) $(SRC)/*~
	$(RM) $(INCLUDE)/*~
	$(RM) doc.tar.gz

install: all
	$(MKDIR) ~/.bin/
	$(CP) $(BIN)/* ~/.bin/

doc: Doxyfile $(SRC)/* $(INCLUDE)/*
	$(MKDIR) $(DOC)
	doxygen
	tar -czf doc.tar.gz doc

edoc: $(BIN)/edoc

debug: edoc
	ddd $(TARGET) $(ARGUMENTS)

leakcheck: edoc
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes $(TARGET) $(ARGUMENTS)

profile: edoc
	valgrind --tool=cachegrind $(TARGET) $(ARGUMENTS)

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	$(MKDIR) $(OBJ)
	$(CC) $(CFLAGS) -o $@ $<

$(INCLUDE)/%.h:
	touch $@

run: edoc
	$(TARGET) $(ARGUMENTS)

trac:
	tracd -s --port 8000 ./.trac
