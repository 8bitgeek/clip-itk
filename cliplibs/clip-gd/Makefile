ifndef CLIPROOT
	CLIPROOT=$(shell cd ../../../; pwd)/cliproot
endif

include $(CLIPROOT)/include/Makefile.inc

CLIPINCLUDE=-I$(CLIPROOT)/include -I./gd
C_FLAGS=-fPIC -Wall -Wextra -g -I.
CC=gcc

XLIB=libclip-gd.a
XSLIB=libclip-gd$(DLLSUFF)
XSLIBREAL=libclip-gd$(DLLREALSUFF)

all: build_gd

build_gd:
	cd gd && ./configure &&	$(MAKE)
	$(CC) $(C_FLAGS) $(CLIPINCLUDE) -c _gd.c
	$(CLIPROOT)/bin/clip -l gd_obj.prg
	cd bsgraphic && $(MAKE)
	rm -f $(XLIB)
	$(CLIPROOT)/bin/clip_makelib $(XLIB) _gd.o gd_obj.o ./bsgraphic/*.o
	./makegd.sh $(XLIB)
	touch build_gd

install:
	$(CLIPROOT)/bin/clip_cp $(XLIB) $(XSLIB) $(DESTDIR)$(CLIPROOT)/lib
	cp ./gd/libgd.a $(DESTDIR)$(CLIPROOT)/lib
	#[ "$(XSLIBREAL)" = "$(XSLIB)" ] || cp $(XSLIBREAL) $(DESTDIR)$(CLIPROOT)/lib
	chmod 0644 $(DESTDIR)$(CLIPROOT)/lib/$(XLIB)
	chmod 0644 $(DESTDIR)$(CLIPROOT)/lib/libgd.a
	chmod 0755 $(DESTDIR)$(CLIPROOT)/lib/$(XSLIB)
	cp gdinfo.ch $(DESTDIR)$(CLIPROOT)/include
	cp bsgraphic/bggraph.ch $(DESTDIR)$(CLIPROOT)/include
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/clip-gd
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/clip-gd/rus
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/clip-gd/eng
	cp ./doc/rus/*.html $(DESTDIR)$(CLIPROOT)/doc/clip-gd/rus
	cp ./doc/rus/*.png $(DESTDIR)$(CLIPROOT)/doc/clip-gd/rus
	cp ./doc/eng/*.html $(DESTDIR)$(CLIPROOT)/doc/clip-gd/eng
	cp ./doc/eng/*.png $(DESTDIR)$(CLIPROOT)/doc/clip-gd/eng
	mkdir -p $(DESTDIR)$(CLIPROOT)/doc/example/clip-gd
	cp -R example/* $(DESTDIR)$(CLIPROOT)/doc/example/clip-gd/

clean:
	-$(MAKE) -C gd clean
	$(MAKE) -C bsgraphic clean
	$(MAKE) -C example clean
	rm -rf *.o gd_obj.c *.s *.bak *.a *.so gd_obj.c *.dll build_gd  *.ex *.nm
