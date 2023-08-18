
SUBDIRS = $(sort gif misc vu0macro fpu)

all: $(patsubst %, _dir_%, $(SUBDIRS)) 

$(patsubst %, _dir_%, $(SUBDIRS)):
	@echo Building$(patsubst _dir_%, %, $@) samples
	@$(MAKE) -r -C $(patsubst _dir_%, %, $@)

clean: $(patsubst %, _cleandir_%, $(SUBDIRS))

$(patsubst %, _cleandir_%, $(SUBDIRS)):
	$(MAKE) -C $(patsubst _cleandir_%, %, $@) clean

