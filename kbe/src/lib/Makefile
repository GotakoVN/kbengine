ifndef KBE_ROOT
export KBE_ROOT := $(subst /kbe/src/lib,,$(CURDIR))
endif

ifndef KBE_CONFIG
	KBE_CONFIG=Hybrid
	ifeq ($(shell uname -m),x86_64)
		 KBE_CONFIG=Hybrid64
	endif
endif

ifneq (,$(findstring 64, $(KBE_CONFIG)))
	OPENSSL_CONFIG+="x86_64=1"
else
	OPENSSL_CONFIG+=
endif

# This directory must match the directory in src/build/common.mak
LIBDIR=$(KBE_ROOT)/kbe/src/lib/bin

all:
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	$(MAKE) _depends
	$(MAKE) _libs

install clean full:
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	cd dependencies/g3dlite && $(MAKE) $@
	cd dependencies/sigar/linux && $(MAKE) $@
	cd dependencies/tmxparser && $(MAKE) $@
	cd dependencies/jwsmtp/jwsmtp/jwsmtp && $(MAKE) $@
	cd client_lib && $(MAKE) $@
	cd common && $(MAKE) $@
	cd db_redis && $(MAKE) $@
	cd db_mysql && $(MAKE) $@
	cd db_interface && $(MAKE) $@
	cd entitydef && $(MAKE) $@
	cd math && $(MAKE) $@
	cd resmgr && $(MAKE) $@
	cd pyscript && $(MAKE) $@
	cd server && $(MAKE) $@
	cd navigation && $(MAKE) $@
	cd network && $(MAKE) $@
	cd helper && $(MAKE) $@
	cd thread && $(MAKE) $@
	cd xml && $(MAKE) $@

#Libs
_libs: $(KBE_ROOT)/kbe/src/libs/libclient_lib.a $(KBE_ROOT)/kbe/src/libs/libcommon.a $(KBE_ROOT)/kbe/src/libs/libdb_redis.a \
		$(KBE_ROOT)/kbe/src/libs/libdb_mysql.a $(KBE_ROOT)/kbe/src/libs/libdb_interface.a $(KBE_ROOT)/kbe/src/libs/libentitydef.a \
		$(KBE_ROOT)/kbe/src/libs/libmath.a $(KBE_ROOT)/kbe/src/libs/libresmgr.a $(KBE_ROOT)/kbe/src/libs/libpyscript.a $(KBE_ROOT)/kbe/src/libs/libserver.a \
		$(KBE_ROOT)/kbe/src/libs/libnavigation.a $(KBE_ROOT)/kbe/src/libs/libnetwork.a $(KBE_ROOT)/kbe/src/libs/libhelper.a \
		$(KBE_ROOT)/kbe/src/libs/libthread.a $(KBE_ROOT)/kbe/src/libs/libxml.a

$(KBE_ROOT)/kbe/src/libs/libclient_lib.a:
	cd client_lib && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libcommon.a:
	cd common && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libdb_redis.a:
	cd db_redis && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libdb_mysql.a:
	cd db_mysql && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libdb_interface.a:
	cd db_interface && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libentitydef.a:
	cd entitydef && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libmath.a:
	cd math && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libresmgr.a:
	cd resmgr && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libpyscript.a:
	cd pyscript && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libserver.a:
	cd server && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libnavigation.a:
	cd navigation && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libnetwork.a:
	cd network && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libhelper.a:
	cd helper && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libthread.a:
	cd thread && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libxml.a:
	cd xml && $(MAKE) $@

#Depends
_depends: $(KBE_ROOT)/kbe/src/libs/libg3dlite.a $(KBE_ROOT)/kbe/src/libs/libsigar.a $(KBE_ROOT)/kbe/src/libs/libtmxparser.a $(KBE_ROOT)/kbe/src/libs/libjwsmtp.a

$(KBE_ROOT)/kbe/src/libs/libg3dlite.a:
	cd dependencies/g3dlite && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libsigar.a:
	cd dependencies/sigar/linux && $(MAKE) $@

$(KBE_ROOT)/kbe/src/libs/libtmxparser.a:
	cd dependencies/tmxparser && $(MAKE) $@
	
$(KBE_ROOT)/kbe/src/libs/libjwsmtp.a:
	cd dependencies/jwsmtp/jwsmtp/jwsmtp && $(MAKE) $@
