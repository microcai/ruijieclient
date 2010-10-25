
bin_PROGRAMS = ruijieclient

ruijieclient_SOURCES = codeconv.h myerr.c ruijieclient.h myerr.h codeconv.c ruijieclient.c
		 

ruijieclient_SOURCES +=prase.c prase.h config.c
 
#ruijieclient_SOURCES +=conn_monitor.c conn_monitor.h
 
ruijieclient_DEPENDENCIES = $(top_builddir)/packet/libruijie.a

ruijieclient_LDADD = $(EXT_LIBS) -ldl $(top_builddir)/packet/libruijie.a
		 
