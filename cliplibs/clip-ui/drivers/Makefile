# This is a part of CLIP-UI library
#
# Copyright (C) 2003-2005 by E/AS Software Foundation
# Author: Andrey Cherepanov <skull@eas.lrn.ru>
ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc
CLIPINCLUDE = -I$(CLIPROOT)/include
CLIP = $(CLIPROOT)/bin/clip
CLIP_MODULE = clip-ui

OBJS    = driver-gtk2.po

.PHONY: all clean uninstall distclean

all: $(OBJS)

clean:
	rm -f $(OBJS) *.bak *.nm *.ex *.ppo *.dll.a *.log *.dll

install: all
	mkdir -p $(DESTDIR)$(CLIPROOT)/lib/drivers
	$(CLIPROOT)/bin/clip_cp $(OBJS) $(DESTDIR)$(CLIPROOT)/lib/drivers

uninstall:
	rm -f $(CLIPROOT)/lib/drivers/$(OBJS)

distclean: clean

dist: distclean

driver-gtk2.po: driver-gtk2.prg
	$(CLIP) $(CLIPINCLUDE) -I../../clip-gtk2 -nw -p driver-gtk2.prg || exit 0
