##############
# setup vars #
##############

export KBE_SRC_ROOT=$(shell cd ../.. && pwd)

KBE_SERVER_BINARIES =	../bin/server/baseapp ../bin/server/baseappmgr \
						../bin/server/cellapp ../bin/server/cellappmgr \
						../bin/server/dbmgr ../bin/server/loginapp \
						../bin/server/machine ../bin/server/kbcmd
				
KBE_LIBS =	libs/libclient_lib.a libs/libcommon.a libs/libdb_redis.a \
			libs/libdb_mysql.a libs/libdb_interface.a libs/libentitydef.a \
			libs/libmath.a libs/libresmgr.a libs/libpyscript.a libs/libserver.a \
			libs/libnavigation.a libs/libnetwork.a libs/libhelper.a \
			libs/libthread.a libs/libxml.a
			
COMMON_LIBS = libs/libhelper.a libs/libthread.a libs/libnetwork.a libs/libcommon.a libs/libserver.a libs/libclient_lib.a

KBE_DEPENDS = libs/libg3dlite.a libs/libsigar.a libs/libtmxparser.a libs/libjwsmtp.a

GET_SRCS=$(shell find $(1) -type f \( -iname \*.cpp -o -iname \*.c -o -iname \*.h -o -iname \*.hpp \) | tr "\n\r" " ")

##################
# default target #
##################

all: $(KBE_SERVER_BINARIES)

#remake faster after fully compiled once
remake:
	$(MAKE) KBE_LIBS="$(COMMON_LIBS)" all

###################
# server/ targets #
###################

BASEAPP_SRCS=$(call GET_SRCS,server/baseapp)
../bin/server/baseapp: $(BASEAPP_SRCS) $(KBE_LIBS) libs/libpyscript.a libs/libresmgr.a libs/libmath.a libs/libentitydef.a
	$(MAKE) -C server/baseapp

BASEAPPMGR_SRCS=$(call GET_SRCS,server/baseappmgr)
../bin/server/baseappmgr: $(BASEAPPMGR_SRCS) $(KBE_LIBS)
	$(MAKE) -C server/baseappmgr

CELLAPP_SRCS=$(call GET_SRCS,server/cellapp)
../bin/server/cellapp: $(CELLAPP_SRCS) $(KBE_LIBS) libs/libnavigation.a libs/libpyscript.a libs/libmath.a libs/libentitydef.a
	$(MAKE) -C server/cellapp

CELLAPPMGR_SRCS=$(call GET_SRCS,server/cellappmgr)
../bin/server/cellappmgr: $(CELLAPPMGR_SRCS) $(KBE_LIBS)
	$(MAKE) -C server/cellappmgr

DBMGR_SRCS=$(call GET_SRCS,server/dbmgr)
../bin/server/dbmgr: $(DBMGR_SRCS) $(KBE_LIBS) libs/libpyscript.a libs/libresmgr.a libs/libmath.a libs/libentitydef.a libs/libdb_interface.a libs/libdb_mysql.a
	$(MAKE) -C server/dbmgr

LOGINAPP_SRCS=$(call GET_SRCS,server/loginapp)
../bin/server/loginapp: $(LOGINAPP_SRCS) $(KBE_LIBS)
	$(MAKE) -C server/loginapp

MACHINE_SRCS=$(call GET_SRCS,server/machine)
../bin/server/machine: $(MACHINE_SRCS) $(KBE_LIBS)
	$(MAKE) -C server/machine
	
#########################
# server/tools/ targets #
#########################

BOTS_SRCS=$(call GET_SRCS,server/tools/bots)
../bin/server/bots: $(BOTS_SRCS) $(KBE_LIBS) libs/libpyscript.a libs/libresmgr.a libs/libentitydef.a
	$(MAKE) -C server/tools/bots

INTERFACES_SRCS=$(call GET_SRCS,server/tools/interfaces)
../bin/server/interfaces: $(INTERFACES_SRCS) $(KBE_LIBS) libs/libresmgr.a
	$(MAKE) -C server/tools/interfaces

KBCMD_SRCS=$(call GET_SRCS,server/tools/kbcmd)
../bin/server/kbcmd: $(KBCMD_SRCS) $(KBE_LIBS) libs/libresmgr.a libs/libentitydef.a
	$(MAKE) -C server/tools/kbcmd

LOGGER_SRCS=$(call GET_SRCS,server/tools/logger)
../bin/server/logger: $(LOGGER_SRCS) $(KBE_LIBS)
	$(MAKE) -C server/tools/logger

################
# lib/ targets #
################

#ls | egrep "\.cpp|\.h" | sed "s/^/$(pwd | sed 's/\/home\/computer\/src\/kbeprojs\/kbengine\/kbe\/src\///g' | sed 's/\//\\\//g')\//g" | tr "\n\r" " " | xsel -ib

#KBE libs
CLIENT_LIB_SRCS=$(call GET_SRCS,lib/client_lib)
libs/libclient_lib.a: $(KBE_DEPENDS) $(CLIENT_LIB_SRCS)
	$(MAKE) -C lib/client_lib

COMMON_SRCS=$(call GET_SRCS,lib/common)
libs/libcommon.a: $(KBE_DEPENDS) $(COMMON_SRCS)
	$(MAKE) -C lib/common

DB_REDIS_SRCS=$(call GET_SRCS,lib/db_redis)
libs/libdb_redis.a: $(KBE_DEPENDS) $(DB_REDIS_SRCS)
	$(MAKE) -C lib/db_redis

DB_MYSQL_SRCS=$(call GET_SRCS,lib/db_mysql)
libs/libdb_mysql.a: $(KBE_DEPENDS) $(DB_MYSQL_SRCS)
	$(MAKE) -C lib/db_mysql

DB_INTERFACE_SRCS=$(call GET_SRCS,lib/db_interface)
libs/libdb_interface.a: $(KBE_DEPENDS) $(DB_INTERFACE_SRCS)
	$(MAKE) -C lib/db_interface

ENTITYDEF_SRCS=$(call GET_SRCS,lib/entitydef)
libs/libentitydef.a: $(KBE_DEPENDS) $(ENTITYDEF_SRCS)
	$(MAKE) -C lib/entitydef

MATH_SRCS=$(call GET_SRCS,lib/math)
libs/libmath.a: $(KBE_DEPENDS) $(MATH_SRCS)
	$(MAKE) -C lib/math

RESMGR_SRCS=$(call GET_SRCS,lib/resmgr)
libs/libresmgr.a: $(KBE_DEPENDS) $(RESMGR_SRCS)
	$(MAKE) -C lib/resmgr

PYSCRIPT_SRCS=$(call GET_SRCS,lib/pyscript)
libs/libpyscript.a: $(KBE_DEPENDS) $(PYSCRIPT_SRCS)
	$(MAKE) -C lib/pyscript

SERVER_SRCS=$(call GET_SRCS,lib/server)
libs/libserver.a: $(KBE_DEPENDS) $(SERVER_SRCS)
	$(MAKE) -C lib/server

NAVIGATION_SRCS=$(call GET_SRCS,lib/navigation)
libs/libnavigation.a: $(KBE_DEPENDS) $(NAVIGATION_SRCS)
	$(MAKE) -C lib/navigation

NETWORK_SRCS=$(call GET_SRCS,lib/network)
libs/libnetwork.a: $(KBE_DEPENDS) $(NETWORK_SRCS)
	$(MAKE) -C lib/network

HELPER_SRCS=$(call GET_SRCS,lib/helper)
libs/libhelper.a: $(KBE_DEPENDS) $(HELPER_SRCS)
	$(MAKE) -C lib/helper

THREAD_SRCS=$(call GET_SRCS,lib/thread)
libs/libthread.a: $(KBE_DEPENDS) $(THREAD_SRCS)
	$(MAKE) -C lib/thread

XML_SRCS=$(call GET_SRCS,lib/xml)
libs/libxml.a: $(KBE_DEPENDS) $(XML_SRCS)
	$(MAKE) -C lib/xml

#Depends

libs/libg3dlite.a:
	$(MAKE) -C lib/dependencies/g3dlite

libs/libsigar.a:
	$(MAKE) -C lib/dependencies/sigar/linux

libs/libtmxparser.a:
	$(MAKE) -C lib/dependencies/tmxparser
	
libs/libjwsmtp.a:
	$(MAKE) -C lib/dependencies/jwsmtp/jwsmtp/jwsmtp

#########
# clean #
#########

clean:
	#lib/
	$(MAKE) -C lib/dependencies/g3dlite clean
	$(MAKE) -C lib/dependencies/sigar/linux clean
	$(MAKE) -C lib/dependencies/tmxparser clean
	$(MAKE) -C lib/dependencies/jwsmtp/jwsmtp/jwsmtp clean
	$(MAKE) -C lib/client_lib clean
	$(MAKE) -C lib/common clean
	$(MAKE) -C lib/db_redis clean
	$(MAKE) -C lib/db_mysql clean
	$(MAKE) -C lib/db_interface clean
	$(MAKE) -C lib/entitydef clean
	$(MAKE) -C lib/math clean
	$(MAKE) -C lib/resmgr clean
	$(MAKE) -C lib/pyscript clean
	$(MAKE) -C lib/server clean
	$(MAKE) -C lib/navigation clean
	$(MAKE) -C lib/network clean
	$(MAKE) -C lib/helper clean
	$(MAKE) -C lib/thread clean
	$(MAKE) -C lib/xml clean
	#server/
	$(MAKE) -C server/baseapp clean
	$(MAKE) -C server/baseappmgr clean
	$(MAKE) -C server/cellapp clean
	$(MAKE) -C server/cellappmgr clean
	$(MAKE) -C server/dbmgr clean
	$(MAKE) -C server/loginapp clean
	$(MAKE) -C server/machine clean
	$(MAKE) -C server/tools clean
