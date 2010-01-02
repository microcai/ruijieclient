AUTOMAKE_OPTIONS = foreign

bin_PROGRAMS = ruijieclient

ruijieclient_SOURCES = codeconv.h myerr.c ruijieclient.h  myerr.h codeconv.c ruijieclient.c \
		 conn_monitor.c conn_monitor.h config.c 

ruijieclient_LDADD = $(EXT_LIBS) -L../packet -lruijie
ruijieclient_CFLAGS := 
	
if USE_XML 
ruijieclient_CFLAGS += -I $(XML_INCLUDE)
 ruijieclient_LDADD += -lxml2
endif	 

if WITH_DBUS
ruijieclient_SOURCES += dbus.c dbus.h 
ruijieclient_CFLAGS += -DUSE_DBUS  ${DBUS_CFLAGS} 
ruijieclient_LDADD += ${DBUS_LIBS}
endif

