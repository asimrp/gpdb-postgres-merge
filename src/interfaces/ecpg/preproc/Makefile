#-------------------------------------------------------------------------
#
# Makefile for src/interfaces/ecpg/preproc
#
# Copyright (c) 1998-2009, PostgreSQL Global Development Group
#
# $PostgreSQL: pgsql/src/interfaces/ecpg/preproc/Makefile,v 1.147 2009/11/27 10:00:40 meskes Exp $
#
#-------------------------------------------------------------------------

PGFILEDESC = "ecpg - embedded SQL precompiler for C"
subdir = src/interfaces/ecpg/preproc
top_builddir = ../../../..
include $(top_builddir)/src/Makefile.global

MAJOR_VERSION= 4
MINOR_VERSION= 6
PATCHLEVEL=0

# The frontend doesn't need everything that's in LIBS, some are backend only
LIBS := $(filter-out -lresolv -lbz2, $(LIBS))
# This program isn't interactive, so doesn't need these
LIBS := $(filter-out -lreadline -ledit -ltermcap -lncurses -lcurses -lcurl -lssl -lcrypto -lz, $(LIBS))

override CPPFLAGS := -I../include -I$(top_srcdir)/src/interfaces/ecpg/include \
	-I. -I$(srcdir) -DMAJOR_VERSION=$(MAJOR_VERSION) \
	-DMINOR_VERSION=$(MINOR_VERSION) -DPATCHLEVEL=$(PATCHLEVEL) \
	 $(CPPFLAGS)

ifeq ($(GCC), yes)
override CFLAGS +=  $(PTHREAD_CFLAGS) -DECPG_COMPILE -Wno-unused-label -Wno-error
endif
override CFLAGS += $(PTHREAD_CFLAGS) -DECPG_COMPILE

OBJS=	preproc.o type.o ecpg.o output.o parser.o \
	keywords.o c_keywords.o ecpg_keywords.o kwlookup.o ../ecpglib/typename.o descriptor.o variable.o \
	$(WIN32RES)

all: submake-libpgport ecpg

ecpg: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LIBS) $(PTHREAD_LIBS) -o $@$(X)

# pgc is compiled as part of preproc
preproc.o: pgc.c

preproc.h: preproc.c ;

preproc.c: preproc.y
ifdef BISON
	$(BISON) -d $(BISONFLAGS) -o $@ $<
else
	@$(missing) bison $< $@
endif

pgc.c: pgc.l
ifdef FLEX
	$(FLEX) $(FLEXFLAGS) -o'$@' $<
else
	@$(missing) flex $< $@
endif

preproc.y: ../../../backend/parser/gram.y parse.pl ecpg.addons ecpg.header ecpg.tokens ecpg.trailer ecpg.type
	$(PERL) $(srcdir)/parse.pl $(srcdir) < $< > $@ 
	$(PERL) $(srcdir)/check_rules.pl $(srcdir) $<

ecpg_keywords.o c_keywords.o keywords.o preproc.o parser.o: preproc.h

kwlookup.c: % : $(top_srcdir)/src/backend/parser/%
	rm -f $@ && $(LN_S) $< .

distprep: preproc.y preproc.c preproc.h pgc.c

install: all installdirs
	$(INSTALL_PROGRAM) ecpg$(X) '$(DESTDIR)$(bindir)'

installdirs:
	$(MKDIR_P) '$(DESTDIR)$(bindir)'

uninstall:
	rm -f '$(DESTDIR)$(bindir)/ecpg$(X)'

clean distclean:
	rm -f kwlookup.c *.o ecpg$(X)
# garbage from partial builds
	@rm -f y.tab.c y.tab.h preproc.tab.c preproc.tab.y preproc.tab.h
# garbage from development
	@rm -f core a.out *.output *.tab.c

# `make distclean' must not remove preproc.y, preproc.c, preproc.h, or pgc.c
# since we want to ship those files in the distribution for people with
# inadequate tools.  Instead, `make maintainer-clean' will remove them.
maintainer-clean: distclean
	rm -f preproc.y preproc.c preproc.h pgc.c
