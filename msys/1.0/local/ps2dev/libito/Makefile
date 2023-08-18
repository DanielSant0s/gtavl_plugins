ifeq (x$(LIBITO), x)
  export LIBITO=$(shell pwd)
endif

ifeq (x$(LIBITO_BIN), x)
  export LIBITO_BIN=$(PS2DEV)/libito
endif

LIBITO_VERSION = 1.0.0

all: build-ito build-tools build-samples

clean:
	$(MAKE) -C tools clean
	$(MAKE) -C samples clean
	$(MAKE) -C source clean
	rm -f -r docs/libref/

build-ito:
	@echo Building libito $(LIBITO_VERSION)
	$(MAKE) -C source

build-samples:
	$(MAKE) -C samples

build-tools:
	@echo Building tools
	$(MAKE) -C tools

build-docs:
	@echo Building docs
	doxygen doxygen/libito.doxygen
	cp -f doxygen/itologo.gif docs/libref/itologo.gif
	cp -f doxygen/ito_manual.css docs/libref/ito_manual.css

bin-release: clean build-ito build-tools build-docs
	rm -f -r release/include/
	mkdir release/include
	for file in `ls include/*.h`; do \
		sed -e s/"%VERSION%"/"$(LIBITO_VERSION)"/ -e s/"%DATE%"/"`date "+%d %B %Y"`"/ release/header > release/$$file; \
		cat $$file>>release/$$file; \
	done;
	echo Creating libito-$(LIBITO_VERSION)-bin.tar.gz
	tar -zcf libito-$(LIBITO_VERSION)-bin.tar.gz docs/ lib/ samples/ tools/iit/iit* tools/iit/readme.txt Makefile.global Makefile.prefab -C release/ Makefile include/  
	rm -f -r release/include/

src-release: clean
	rm -f -r release/include/
	echo Creating libito-$(LIBITO_VERSION)-src.tar.gz
	tar -zcf libito-$(LIBITO_VERSION)-src.tar.gz docs/ doxygen/ include/ lib/ release/ samples/ source/ tools/ Makefile Makefile.global MakeFile.prefab

install:
	mkdir -p $(LIBITO_BIN)/lib $(LIBITO_BIN)/include $(LIBITO_BIN)/bin
	cp include/*.h $(LIBITO_BIN)/include
	cp lib/*.a $(LIBITO_BIN)/lib
	cp tools/iit/iit* $(LIBITO_BIN)/bin
