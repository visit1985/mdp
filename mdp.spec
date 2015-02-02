Name:		mdp
Version:	0.93.0
Release:	1%{?dist}
Summary:	A command-line based markdown presentation tool

Group:		Applications
License:	GPL
URL:		https://github.com/visit1985/mdp
Source0:	https://github.com/visit1985/mdp/archive/0.93.0.tar.gz

BuildRequires:	gcc make ncurses-devel
Requires:	glibc

%description
mdp is an application for running a presentation on
the CLI with colors and different sizes and fonts

%prep
%setup -q


%build
%configure --prefix=/usr --sysconfdir=/etc
make %{?_smp_mflags}


%install
rm -rf %{buildroot}
make install DESTDIR=%{buildroot}


%files
%{_bindir}/mdp
%doc


%changelog

