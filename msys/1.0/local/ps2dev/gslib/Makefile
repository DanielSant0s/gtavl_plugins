# Remove the line below, if you want to disable silent mode
#.SILENT:

all: build-gslib build-examples

clean:
	$(MAKE) -C examples clean
	$(MAKE) -C source clean

build-gslib:
	@echo Building gslib
	$(MAKE) -C source

build-examples:
	@echo Building examples
	$(MAKE) -C examples
