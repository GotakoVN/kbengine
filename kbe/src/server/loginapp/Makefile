BIN  = loginapp
SRCS =						\
	http_cb_handler				\
	loginapp				\
	loginapp_interface			\
	main					\
	profile

ASMS =

MY_LIBS =		\
	server		\
	network		\
	pyscript	\
	thread
	

BUILD_TIME_FILE = main
USE_PYTHON = 1
USE_G3DMATH = 1
USE_OPENSSL = 1
USE_JWSMTP = 1

ifndef NO_USE_LOG4CXX
	NO_USE_LOG4CXX = 0
	CPPFLAGS += -DLOG4CXX_STATIC
endif

#HAS_PCH = 1
CPPFLAGS += -DKBE_LOGINAPP

include $(KBE_SRC_ROOT)/kbe/src/build/common.mak

