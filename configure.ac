#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ruijieclient], [0.8.3], [gong@fedoraproject.org])

AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_ARG_ENABLE(debug,
                [  --enable-debug Enable DEBUG output. This is a debugging feature  which should not be only enabled by develpoers],
                [ CFLAGS="$CFLAGS -O0 -DDEBUG" ],
                [ CFLAGS="$CFLAGS -g0"]
              )

AC_ARG_ENABLE(dyload,
			[AC_HELP_STRING([--diable-dyload],
			[link to libpcap.so instead of lookup libpcap.so when program starts])],
			[with_dyload=$enableval],
			[with_dyload=yes])

AS_IF( [ test "x$with_dyload" = "xyes"  ],[
	AC_DEFINE(USE_DYLIBPCAP,1,[Use dlopen to load libpcap])	
])

AC_CHECK_LIB(pcap,pcap_close,
		[
			AS_IF( [ test "x$with_dyload" != "xyes"  ],[LIBS+="-lpcap"],)
		],
		[
			AS_IF( [ test "x$with_dyload" != "xyes"  ], [ AC_MSG_FAILURE([libpcap-dev no found],[1]) ])
		])

AC_C_BIGENDIAN()

# Checks for library functions.
AC_CHECK_FUNCS([memset strerror])

AC_CHECK_HEADER([netinet/in.h])
AC_CHECK_DECL([INET_ADDRSTRLEN],[],[AC_DEFINE([INET_ADDRSTRLEN],[16],[sizeof(sockaddr_in)])],
				[[ #include <netinet/in.h>
				]])

AC_CHECK_HEADERS([iconv.h  arpa/inet.h stdlib.h string.h sys/socket.h sys/time.h unistd.h net/ethernet.h])

AC_CHECK_HEADERS([linux/rtnetlink.h],[],[],
	[[ #include <sys/types.h>
	   #include <sys/socket.h>	
	]])
AC_CHECK_HEADERS([linux/rtnetlink.h])
# Checks for libraries.

AC_CHECK_HEADER([pcap/pcap.h],[],[AC_MSG_FAILURE([pcap.h not found!],1)])
	
AC_CONFIG_FILES([Makefile
				 packet/Makefile
				 src/Makefile])
AC_OUTPUT
