#-------------------------------------------------------------------------
#
# Makefile for src/tools/ifaddrs
#
# Copyright (c) 2003-2009, PostgreSQL Global Development Group
#
# $PostgreSQL: pgsql/src/tools/ifaddrs/Makefile,v 1.1 2009/10/01 01:58:58 tgl Exp $
#
#-------------------------------------------------------------------------

subdir = src/tools/ifaddrs
top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

libpq_backend_dir = $(top_builddir)/src/backend/libpq

override CPPFLAGS := -I$(libpq_backend_dir) $(CPPFLAGS)

OBJS = test_ifaddrs.o

all: test_ifaddrs

test_ifaddrs: test_ifaddrs.o $(libpq_backend_dir)/ip.o
	$(CC) $(CFLAGS) test_ifaddrs.o $(libpq_backend_dir)/ip.o $(LDFLAGS) $(LIBS) -o $@$(X)

clean distclean maintainer-clean:
	rm -f test_ifaddrs$(X) $(OBJS)
