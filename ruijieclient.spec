%define build_number 1
%define debug_package %{nil}

Name:      ruijieclient
Version:   0.8.2
Release:   %{build_number}%{?dist}
Summary:   a ruijie network supplicant for GNU/Linux

Group:     Applications/Internet
License:   LGPLv2+
URL:       http://code.google.com/p/ruijieclient/
Source0:   http://ruijieclient.googlecode.com/files/ruijieclient-%{version}.tar.bz2
BuildRoot: %{_tmppath}/%{name}_%{version}-%{release}-root_%(%{__id_u} -n)

BuildRequires: libpcap-devel glibc-devel libxml2-devel
Requires: libpcap glibc kernel libxml2

%description
RuijieClient is a ruijie network supplicant for GNU/Linux  which is based on mystar, but re-writed form scratch.

%prep
%setup -q

%build
%configure
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT

make install DESTDIR=$RPM_BUILD_ROOT
chmod 4755 $RPM_BUILD_ROOT/usr/bin/ruijieclient

%clean
rm -rf $RPM_BUILD_ROOT

%post
update-desktop-database %{_datadir}/applications &>/dev/null || :


%postun
update-desktop-database %{_datadir}/applications &>/dev/null || :

%files
%defattr(-,root,root,-)
%{_prefix}/bin/ruijieclient
%{_docdir}/ruijieclient/README

%changelog

* Sat Jun 13 2009 Gong Han <gong@fedoraproject.org> - 0.8-1
- Creat a spec file
