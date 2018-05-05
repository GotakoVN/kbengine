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
LIBDIR=$(KBE_SRC_ROOT)/kbe/src/lib/bin

all:
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	$(MAKE) _depends
	$(MAKE) _libs

clean:
	test -d $(LIBDIR) || mkdir $(LIBDIR)
	cd dependencies/g3dlite && $(MAKE) clean
	cd dependencies/sigar/linux && $(MAKE) clean
	cd dependencies/tmxparser && $(MAKE) clean
	cd dependencies/jwsmtp/jwsmtp/jwsmtp && $(MAKE) clean
	cd client_lib && $(MAKE) clean
	cd common && $(MAKE) clean
	cd db_redis && $(MAKE) clean
	cd db_mysql && $(MAKE) clean
	cd db_interface && $(MAKE) clean
	cd entitydef && $(MAKE) clean
	cd math && $(MAKE) clean
	cd resmgr && $(MAKE) clean
	cd pyscript && $(MAKE) clean
	cd server && $(MAKE) clean
	cd navigation && $(MAKE) clean
	cd network && $(MAKE) clean
	cd helper && $(MAKE) clean
	cd thread && $(MAKE) clean
	cd xml && $(MAKE) clean

install full:
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
_libs: $(KBE_SRC_ROOT)/kbe/src/libs/libclient_lib.a $(KBE_SRC_ROOT)/kbe/src/libs/libcommon.a $(KBE_SRC_ROOT)/kbe/src/libs/libdb_redis.a \
		$(KBE_SRC_ROOT)/kbe/src/libs/libdb_mysql.a $(KBE_SRC_ROOT)/kbe/src/libs/libdb_interface.a $(KBE_SRC_ROOT)/kbe/src/libs/libentitydef.a \
		$(KBE_SRC_ROOT)/kbe/src/libs/libmath.a $(KBE_SRC_ROOT)/kbe/src/libs/libresmgr.a $(KBE_SRC_ROOT)/kbe/src/libs/libpyscript.a $(KBE_SRC_ROOT)/kbe/src/libs/libserver.a \
		$(KBE_SRC_ROOT)/kbe/src/libs/libnavigation.a $(KBE_SRC_ROOT)/kbe/src/libs/libnetwork.a $(KBE_SRC_ROOT)/kbe/src/libs/libhelper.a \
		$(KBE_SRC_ROOT)/kbe/src/libs/libthread.a $(KBE_SRC_ROOT)/kbe/src/libs/libxml.a

$(KBE_SRC_ROOT)/kbe/src/libs/libclient_lib.a:
	cd client_lib && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libcommon.a:
	cd common && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libdb_redis.a:
	cd db_redis && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libdb_mysql.a:
	cd db_mysql && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libdb_interface.a:
	cd db_interface && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libentitydef.a:
	cd entitydef && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libmath.a:
	cd math && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libresmgr.a:
	cd resmgr && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libpyscript.a:
	cd pyscript && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libserver.a:
	cd server && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libnavigation.a:
	cd navigation && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libnetwork.a:
	cd network && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libhelper.a:
	cd helper && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libthread.a:
	cd thread && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libxml.a:
	cd xml && $(MAKE) $@

#Depends
_depends: $(KBE_SRC_ROOT)/kbe/src/libs/libg3dlite.a $(KBE_SRC_ROOT)/kbe/src/libs/libsigar.a $(KBE_SRC_ROOT)/kbe/src/libs/libtmxparser.a $(KBE_SRC_ROOT)/kbe/src/libs/libjwsmtp.a

$(KBE_SRC_ROOT)/kbe/src/libs/libg3dlite.a:
	cd dependencies/g3dlite && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libsigar.a:
	cd dependencies/sigar/linux && $(MAKE) $@

$(KBE_SRC_ROOT)/kbe/src/libs/libtmxparser.a:
	cd dependencies/tmxparser && $(MAKE) $@
	
$(KBE_SRC_ROOT)/kbe/src/libs/libjwsmtp.a:
	cd dependencies/jwsmtp/jwsmtp/jwsmtp && $(MAKE) $@
