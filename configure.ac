#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([ruijieclient], [0.8.2 svn-r136], [gong@fedoraproject.org])

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.

AC_CANONICAL_TARGET

AC_PROG_CC
AC_PROG_RANLIB

AM_INIT_AUTOMAKE([-Wall -Werror])

AC_PROG_INSTALL

AC_SUBST(have_xml,"no")
AC_SUBST(have_pcap,"no")

AC_ARG_ENABLE(dyload,[AC_HELP_STRING([--enable-dyload],[do not link to libpcap.so, but lookup libpcap.so when program starts])],
		[],[enable_dyload="no"]
		)
AC_ARG_WITH([dbus],
	[AC_HELP_STRING([--with-dbus],[use dbus to cooperate with NetworkManager and auto-auth when network cable plugin])],
	[PKG_PROG_PKG_CONFIG],[PKG_PROG_PKG_CONFIG
	with_dbus="yes"]
	)
	
AC_ARG_WITH([conf],
	[AC_HELP_STRING([--with-conf=/etc/ruijie.conf],[use this to override default config file that used])],
	[],[])

AC_ARG_WITH([xml],
		[AC_HELP_STRING([--with-xml,--with-xml=HEADERDIR],[use libxml2 to prase config files, if not install at /usr/include/libxml2, please get the installation dir])],
		[],[with_xml="no"])

AC_ARG_ENABLE(debug,
		[AC_HELP_STRING([--enable-debug],
                			[Enable DEBUG output. This is a debugging feature  which should not be only enabled by develpoers])],
                [],
                [ enable_debug="no"]
          )

# Checks for typedefs, structures, and compiler characteristics.

AC_C_BIGENDIAN(
	[AC_DEFINE(WORDS_BIGENDIAN,[1],
		[Define WORDS_BIGENDIAN to 1 if your processor stores words with the most significant
    byte first (like Motorola and SPARC, unlike Intel).])],
	[AC_MSG_RESULT([little ending cpu detected!])],
	[AC_MSG_ERROR([cannot detect cpu ending],[1])]
)

# Checks for library functions.
AC_CHECK_FUNCS([memset strerror])

AC_CHECK_HEADER(pcap.h,[], [AC_MSG_FAILURE( libpcap development files not found!, 1)])


AC_CHECK_HEADERS([iconv.h  arpa/inet.h netinet/in.h stdlib.h string.h sys/socket.h sys/time.h unistd.h net/ethernet.h])


AC_CHECK_HEADER([netinet/in.h])

AC_CHECK_DECL([INET_ADDRSTRLEN],[],[AC_DEFINE([INET_ADDRSTRLEN],[16],[sizeof(sockaddr_in)])],
				[[ #include <netinet/in.h>
				]])

AC_CHECK_HEADERS([iconv.h  arpa/inet.h stdlib.h string.h sys/socket.h sys/time.h unistd.h net/ethernet.h])

AC_CHECK_HEADER([net/if_dl.h])

AC_CHECK_HEADER([linux/rtnetlink.h],[],[],
	[[ #include <sys/types.h>
	   #include <sys/socket.h>	
	]])

# Checks for libraries.
AS_IF( [ test x"$enable_dyload" != xno] ,
		 [
		 	AC_CHECK_LIB(pcap,
		 			[pcap_open_live],[LIBS=-lpcap],
		 			[AC_MSG_FAILURE([libpcap development files not found!],[1])]
		 	)
		 ]
  )

# important ! here we just determin where the user have got libxml2-devel installed
AS_IF( [ test ! x"$with_xml" = xno ],
	[
	    AC_CHECK_LIB(xml2, [main],
	    	[	    	
	    		have_xml="yes"
	    		
	    		AS_IF( [ test x"$with_xml" = xyes ],
	    			[XML_INCLUDE="-I/usr/include/libxml2"],
	    			[XML_INCLUDE="-I$with_xml"])
	    	],
	    	[
	    		AC_WARN([libxml2 not found! xml feature will not work!])
	    	])
	]
)



AC_SUBST_FILE(XML_INCLUDE)

AM_CONDITIONAL(USE_XML,[ test "x$have_xml" = "xyes" ])

AS_IF( [ test "x$have_xml" = "xyes" ] ,[ AC_DEFINE([HAVE_LIBXML2],1,[Define to 1 if you have the libxml2 header file])])

AS_IF( [test "x$enable_debug" = "xyes" ] ,[CFLAGS=" -g3 -O0"])

AS_IF( [ test "x$with_dbus" = "xyes" ],[
	PKG_CHECK_MODULES(DBUS,[ dbus-glib-1 ])
	AC_DEFINE([USE_DBUS],[1],[defined if enabled dbus support])
])

AM_CONDITIONAL(WITH_DBUS,[test "x$with_dbus" = "xyes"]) 

# Checks for header files.

AC_CONFIG_FILES([Makefile
                 packet/Makefile
                 configer/Makefile
           		])
AC_OUTPUT
