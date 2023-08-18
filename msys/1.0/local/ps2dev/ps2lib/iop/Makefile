all: lib samples utils

lib: build-libkernel

utils: build-libkernel
	$(MAKE) -C utils

clean:
	$(MAKE) -C libkernel clean
	$(MAKE) -C lib clean
	$(MAKE) -C sample clean
	$(MAKE) -C utils clean

build-libkernel:
	$(MAKE) -C libkernel

samples: lib
	$(MAKE) -C sample


include ../Makefile.pref
include ../Makefile.iopglobal
