LIB =	server

SRCS =				\
	common			\
	components		\
	component_active_report_handler \
	entity_app		\
	forward_messagebuffer	\
	globaldata_client	\
	globaldata_server	\
	idallocate		\
	pendingLoginmgr		\
	py_file_descriptor	\
	python_app		\
	script_timers		\
	serverapp		\
	serverconfig		\
	machine_infos		\
	sendmail_threadtasks	\
	shutdowner		\
	signal_handler		\
	telnet_handler		\
	telnet_server		\
	python_app

USE_PYTHON=1
USE_JWSMTP = 1

ifndef NO_USE_LOG4CXX
	NO_USE_LOG4CXX = 0
	CPPFLAGS += -DLOG4CXX_STATIC
endif

include $(KBE_SRC_ROOT)/kbe/src/build/common.mak
