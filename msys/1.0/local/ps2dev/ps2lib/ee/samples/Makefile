all: build-kernel build-hello build-pad build-memcard

build-kernel:
	$(MAKE) -C kernel

build-hello:
	$(MAKE) -C hello

build-pad:
	$(MAKE) -C pad

build-memcard:
	$(MAKE) -C memcard

clean:
	$(MAKE) -C kernel clean
	$(MAKE) -C hello clean
	$(MAKE) -C pad clean
	$(MAKE) -C memcard clean
