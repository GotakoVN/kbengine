all clean realclean all_config install unit_tests unit_tests_build unit_tests_run unit_tests_clean::

# Check with and without the trailing slash
ifneq ($(CURDIR),$(KBE_SRC_ROOT)/kbe/src/server/tools)
ifneq ($(CURDIR),$(KBE_SRC_ROOT)kbe/src/server/tools)
	@echo 'Error: KBE_SRC_ROOT=$(KBE_SRC_ROOT)'
	@echo '       is not the root of $(CURDIR)'
	@echo 'Remove the KBE_SRC_ROOT environment variable.'
	@false
endif
endif

	$(MAKE) -C ../../lib $@
	$(MAKE) -C bots $@
	$(MAKE) -C interfaces $@
	$(MAKE) -C logger $@
	$(MAKE) -C kbcmd $@

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
