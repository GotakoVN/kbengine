ifndef KBE_ROOT
export KBE_ROOT := $(subst /kbe/src/server,,$(CURDIR))
endif

all clean realclean all_config install::

# Check with and without the trailing slash
ifneq ($(CURDIR),$(KBE_ROOT)/kbe/src/server)
ifneq ($(CURDIR),$(KBE_ROOT)kbe/src/server)
	@echo 'Error: KBE_ROOT=$(KBE_ROOT)'
	@echo '       is not the root of $(CURDIR)'
	@echo 'Remove the KBE_ROOT environment variable.'
	@false
endif
endif

	$(MAKE) _all_server

ifdef KBE_CONFIG
	@echo completed $@ \(KBE_CONFIG = $(KBE_CONFIG)\)
else
	@echo completed $@
endif
	$(MAKE) done

done:
ifdef DO_NOT_BELL
else
	echo -n 
endif

server:
	$(MAKE) all

_all_server: $(KBE_ROOT)/bin/server/baseapp $(KBE_ROOT)/bin/server/baseappmgr $(KBE_ROOT)/bin/server/cellapp \
			$(KBE_ROOT)/bin/server/cellappmgr $(KBE_ROOT)/bin/server/dbmgr $(KBE_ROOT)/bin/server/loginapp \
			$(KBE_ROOT)/bin/server/machine $(KBE_ROOT)/bin/server/kbcmd

$(KBE_ROOT)/bin/server/baseapp:
	$(MAKE) -C baseapp

$(KBE_ROOT)/bin/server/baseappmgr:
	$(MAKE) -C baseappmgr

$(KBE_ROOT)/bin/server/cellapp:
	$(MAKE) -C cellapp

$(KBE_ROOT)/bin/server/cellappmgr:
	$(MAKE) -C cellappmgr

$(KBE_ROOT)/bin/server/dbmgr:
	$(MAKE) -C dbmgr

$(KBE_ROOT)/bin/server/loginapp:
	$(MAKE) -C loginapp

$(KBE_ROOT)/bin/server/machine:
	$(MAKE) -C machine

$(KBE_ROOT)/bin/server/kbcmd:
	$(MAKE) -C tools
