LIB =	helper

SRCS =				\
	debug_helper		\
	debug_option		\
	eventhistory_stats	\
	profile			\
	profiler		\
	profile_handler		\
	script_loglevel		\
	sys_info		\
	watch_pools		\
	watcher

ifndef NO_USE_LOG4CXX
	NO_USE_LOG4CXX = 0
	CPPFLAGS += -DLOG4CXX_STATIC
endif

include $(KBE_SRC_ROOT)/kbe/src/build/common.mak
