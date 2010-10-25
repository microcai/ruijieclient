SUBDIRS = packet src
dist_doc_DATA = README

EXTRA_DIST=ruijieclient.spec.in

DISTCLEANFILES = ruijieclient.spec 
CLEANFILES = config.h.in~

spec=@PACKAGE_NAME@.spec

love:
	@ echo ' HI, make love ing ......'
	@ echo '    ***        ***'
	@ echo '  **    **  **    **'
	@ echo ' **       *        **'
	@ echo '  **              **'
	@ echo '   **            **'
	@ echo '     **        **'
	@ echo '        ** **'
	@ echo '          *'
	@ echo Make love complete

rpm: dist @PACKAGE_NAME@.spec
	rpmbuild -bb \
			--define "_sourcedir `pwd`" \
			--define "_builddir `pwd`" \
			--define "_specdir `pwd`" \
			--define "_rpmdir `pwd`" \
			--define "_srcrpmdir `pwd`" \
			$(spec)

srpm: dist @PACKAGE_NAME@.spec
	rpmbuild -bs \
			--define "_sourcedir `pwd`" \
			--define "_builddir `pwd`" \
			--define "_srcrpmdir `pwd`" \
		    --define "_rpmdir `pwd`" \
			--define "_specdir `pwd`" \
			$(spec)
				