all: lib samples

lib: build-libkernel build-startup build-libpad build-libmc

clean:
	$(MAKE) -C libkernel clean
	$(MAKE) -C lib clean
	$(MAKE) -C samples clean
	$(MAKE) -C startup clean
	$(MAKE) -C libpad clean
	$(MAKE) -C libmc clean

build-libkernel:
	$(MAKE) -C libkernel

build-startup:
	$(MAKE) -C startup

build-libpad:
	$(MAKE) -C libpad

build-libmc:
	$(MAKE) -C libmc

samples: lib
	$(MAKE) -C samples

include ../Makefile.pref
include ../Makefile.eeglobal
