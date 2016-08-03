Name:    krate
Version: 0.0.1
Release: 0
License: Apache-2.0
Source0: file://%{name}-%{version}.tar.gz
Summary: Tizen Krate Manager
Group:   Security/Other
Requires: systemd
BuildRequires: pam-devel
BuildRequires: gcc
BuildRequires: cmake
BuildRequires: gettext-tools
BuildRequires: pkgconfig(klay)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(pkgmgr)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(appsvc)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(capi-system-system-settings)
BuildRequires: pkgconfig(notification)
BuildRequires: pkgconfig(key-manager)
BuildRequires: pkgconfig(auth-fw-admin)
BuildRequires: pkgconfig(cynara-client)
BuildRequires: pkgconfig(cynara-session)
BuildRequires: pkgconfig(libgum)

%description
The krate package provides a daemon which is responsible for managing each of
krates.

%files
%manifest krate.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/krated
%attr(700,root,root) %{_sbindir}/krate-volume-manager
%{_unitdir}/krate.service
%{_unitdir}/multi-user.target.wants/krate.service
%attr(700,root,root) /etc/gumd/useradd.d/20_krate-add.post
%attr(700,root,root) /etc/gumd/userdel.d/20_krate-remove.post
%attr(644,root,root) %{TZ_SYS_RO_ICONS}/krate/indicator_icon.png
%attr(644,root,root) %{TZ_SYS_RO_ICONS}/krate/notification_sub_icon.png
%attr(700,root,root) %dir %{TZ_SYS_ETC}/krate
%attr(600,root,root) %config %{TZ_SYS_ETC}/krate/owner.xml
%attr(700,root,root) %{_libdir}/security/pam_*.so
%attr(700,root,root) %{_sbindir}/krate-admin-cli
%config /etc/pam.d/*

%prep
%setup -q

%build
%{!?build_type:%define build_type "RELEASE"}

%if %{build_type} == "DEBUG" || %{build_type} == "PROFILING" || %{build_type} == "CCOV"
	CFLAGS="$CFLAGS -Wp,-U_FORTIFY_SOURCE"
	CXXFLAGS="$CXXFLAGS -Wp,-U_FORTIFY_SOURCE"
%endif

%{!?profile:%define profile "mobile"}

%cmake . -DVERSION=%{version} \
         -DCMAKE_BUILD_TYPE=%{build_type} \
         -DTIZEN_PROFILE_NAME=%{profile} \
         -DRUN_DIR=%{TZ_SYS_RUN} \
         -DBIN_DIR=%{TZ_SYS_BIN} \
         -DHOME_DIR=%{TZ_SYS_HOME} \
         -DSYSTEMD_UNIT_DIR=%{_unitdir} \
         -DPAMD_DIR=/etc/pam.d \
         -DCONF_DIR=%{TZ_SYS_ETC}/krate \
         -DICON_DIR="%{TZ_SYS_RO_ICONS}/krate" \
         -DAPP_INSTALL_PREFIX="%{TZ_SYS_RO_APP}" \
         -DAPP_SHARE_PACKAGES_DIR="%{TZ_SYS_RO_PACKAGES}"

make %{?jobs:-j%jobs}

%install
%make_install
mkdir -p %{buildroot}/%{_unitdir}/multi-user.target.wants
ln -s ../krate.service %{buildroot}/%{_unitdir}/multi-user.target.wants/krate.service

%post
mv /etc/pam.d/systemd-user /etc/pam.d/systemd-user.keep
cp /etc/pam.d/systemd-user-krate /etc/pam.d/systemd-user

%clean
rm -rf %{buildroot}

%preun

%postun
mv /etc/pam.d/systemd-user.keep /etc/pam.d/systemd-user

## Krate Client Package ########################################################
%package -n libkrate
Summary: Tizen Krate Client library
Group: Development/Libraries
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(libtzplatform-config)
Requires: %{name} = %{version}-%{release}
Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig

%description -n libkrate
The libkrate package contains the libraries needed to control inside of the krate.

%post -n libkrate -p /sbin/ldconfig

%postun -n libkrate -p /sbin/ldconfig

%files -n libkrate
%manifest krate.manifest
%defattr(644,root,root,755)
%attr(755,root,root) %{_libdir}/libkrate.so.%{version}
%{_libdir}/libkrate.so.0

## Devel Package ##############################################################
%package -n libkrate-devel
Summary: Libraries and header files for krate client development
Group: Development/Libraries
Requires: libkrate = %{version}-%{release}

%description -n libkrate-devel
The libkrate-devel package includes the libraries and header files necessary for
developing the krate client program.

%files -n libkrate-devel
%manifest krate.manifest
%defattr(644,root,root,755)
%{_libdir}/libkrate.so
%{_includedir}/krate
%{_libdir}/pkgconfig/krate.pc

## Krate Setup Wizard Package #################################################
%package -n org.tizen.krate-setup-wizard
Summary: Tizen Krate setup wizard Interface
Group: Security/Other
Requires: libkrate = %{version}-%{release}
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(notification)

%description -n org.tizen.krate-setup-wizard
Tizen Krate setup wizard interface

%define setup_home %{TZ_SYS_RO_APP}/org.tizen.krate-setup-wizard

%files -n org.tizen.krate-setup-wizard
%defattr(-,root,root,-)
%manifest tools/apps/setup-wizard/org.tizen.krate-setup-wizard.manifest
%{setup_home}/bin/*
%{setup_home}/res/*
%{TZ_SYS_RO_PACKAGES}/org.tizen.krate-setup-wizard.xml

## Kaskit package #####################################################
%package -n org.tizen.kaskit
Summary: Tizen Krate launcher Interface
Group: Security/Other
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(capi-appfw-package-manager)
BuildRequires: pkgconfig(capi-appfw-app-manager)
BuildRequires: pkgconfig(badge)

%description -n org.tizen.kaskit
Tizen Krate launcher interface

%define launcher_home %{TZ_SYS_RO_APP}/org.tizen.kaskit

%files -n org.tizen.kaskit
%defattr(-,root,root,-)
%manifest tools/apps/kaskit/org.tizen.kaskit.manifest
%{launcher_home}/bin/*
%{launcher_home}/res/*
%{TZ_SYS_RO_PACKAGES}/org.tizen.kaskit.xml

## Keyguard package #####################################################
%package -n org.tizen.keyguard
Summary: Tizen Krate keyguard Interface
Group: Security/Other
BuildRequires: pkgconfig(efl-extension)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-appfw-application)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(capi-ui-efl-util)
BuildRequires: pkgconfig(auth-fw)
BuildRequires: pkgconfig(shortcut)

%description -n org.tizen.keyguard
Tizen Krate keyguard interface

%define keyguard_home %{TZ_SYS_RO_APP}/org.tizen.keyguard

%files -n org.tizen.keyguard
%defattr(-,root,root,-)
%manifest tools/apps/keyguard/org.tizen.keyguard.manifest
%{keyguard_home}/bin/*
%{keyguard_home}/res/*
%{TZ_SYS_RO_PACKAGES}/org.tizen.keyguard.xml
