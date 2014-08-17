# Copyright 2014 Luke Dashjr
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the standard MIT license.  See COPYING for more details.

lib_LTLIBRARIES = libbase58.la
libbase58_la_SOURCES = base58.c
libbase58_la_LDFLAGS = -version-info $(LIBBASE58_SO_VERSION) -no-undefined

libbase58_includedir = $(includedir)
libbase58_include_HEADERS = base58.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libbase58.pc

dist_noinst_SCRIPTS = autogen.sh
dist_doc_DATA = README
