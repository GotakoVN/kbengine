LIB =	client_lib

SRCS =					\
	client_interface	\
	clientapp			\
	clientobjectbase	\
	config				\
	script_callbacks	\
	entity				\
	entity_aspect		\
	entity_component	\
	event				\
	moveto_point_handler\
	profile

USE_PYTHON=1
USE_OPENSSL=1

ifndef NO_USE_LOG4CXX
	NO_USE_LOG4CXX = 0
	CPPFLAGS += -DLOG4CXX_STATIC
endif

include $(KBE_SRC_ROOT)/kbe/src/build/common.mak
