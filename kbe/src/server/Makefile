all realclean all_config install::

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

_all_server: $(KBE_SRC_ROOT)/bin/server/baseapp $(KBE_SRC_ROOT)/bin/server/baseappmgr $(KBE_SRC_ROOT)/bin/server/cellapp \
			$(KBE_SRC_ROOT)/bin/server/cellappmgr $(KBE_SRC_ROOT)/bin/server/dbmgr $(KBE_SRC_ROOT)/bin/server/loginapp \
			$(KBE_SRC_ROOT)/bin/server/machine $(KBE_SRC_ROOT)/bin/server/kbcmd

$(KBE_SRC_ROOT)/bin/server/baseapp:
	$(MAKE) -C baseapp

$(KBE_SRC_ROOT)/bin/server/baseappmgr:
	$(MAKE) -C baseappmgr

$(KBE_SRC_ROOT)/bin/server/cellapp:
	$(MAKE) -C cellapp

$(KBE_SRC_ROOT)/bin/server/cellappmgr:
	$(MAKE) -C cellappmgr

$(KBE_SRC_ROOT)/bin/server/dbmgr:
	$(MAKE) -C dbmgr

$(KBE_SRC_ROOT)/bin/server/loginapp:
	$(MAKE) -C loginapp

$(KBE_SRC_ROOT)/bin/server/machine:
	$(MAKE) -C machine

$(KBE_SRC_ROOT)/bin/server/kbcmd:
	$(MAKE) -C tools
	
clean:
	$(MAKE) -C baseapp clean
	$(MAKE) -C baseappmgr clean
	$(MAKE) -C cellapp clean
	$(MAKE) -C cellappmgr clean
	$(MAKE) -C dbmgr clean
	$(MAKE) -C loginapp clean
	$(MAKE) -C machine clean
	$(MAKE) -C tools clean
