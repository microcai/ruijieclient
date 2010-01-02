#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ruijieclient], [0.8.2], [gong@fedoraproject.org])

AM_INIT_AUTOMAKE([-Wall -Werror])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

USE_DYLIB=0

AC_ARG_ENABLE(dyload,
			[  --enable-dyload do not link to libpcap.so ,
			                    but lookup libpcap.so when program starts],
			[USE_DYLIB=1],
			[USE_DYLIB=0])

AC_CHECK_LIB([pcap], [main],
			[
			
			if [$USE_DYLIB=0] ; then 
			  LIBS+=-lpcap
			else
			 CFLAGS+=" -DUSE_DYLIB "			 				
			fi			
			],      
            [AC_MSG_FAILURE( libpcap not found!,1)])


# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_TYPE_UINT8_T

AC_C_BIGENDIAN()

# Checks for library functions.
AC_CHECK_FUNCS([memset strerror])

AC_CHECK_HEADER([netinet/in.h])
AC_CHECK_DECL([INET_ADDRSTRLEN],[],[AC_DEFINE([INET_ADDRSTRLEN],[16],[sizeof(sockaddr_in)])],
				[[ #include <netinet/in.h>
				]])

AC_CHECK_HEADERS([iconv.h  arpa/inet.h stdlib.h string.h sys/socket.h sys/time.h unistd.h net/ethernet.h])
AC_CHECK_HEADER([linux/rtnetlink.h],[],[],
	[[ #include <sys/types.h>
	   #include <sys/socket.h>	
	]])

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lpcap':

AC_CHECK_HEADER(pcap.h,[],
			AC_MSG_FAILURE( libpcap development files not found!, 1 ))
			
# important ! here we just determin where the user have got libxml2-devel installed

AC_CHECK_LIB(xml2, [main],[],[AC_WARN(libxml2 not found! xml feature will not work!)])
	
AC_SUBST(XML_INCLUDE,/usr/include/libxml2) 
	
AC_SUBST_FILE(XML_INCLUDE)

# Checks for header files.

AC_ARG_ENABLE(debug,
                [  --enable-debug Enable DEBUG output. This is a debugging feature  which should not be only enabled by develpoers],
                [ CFLAGS="$CFLAGS -g2 -O0 -DDEBUG" ],
                [ CFLAGS="$CFLAGS -g0 -Os"]
              )
             
 	
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
