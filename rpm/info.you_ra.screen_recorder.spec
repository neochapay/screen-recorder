%define __provides_exclude ^lib.*\\.*$
%define __requires_exclude ^.*$

Name:       info.you_ra.screen_recorder

Summary:    Screen recorder application
Version:    0.1.302820
Release:    1
Group:      Qt/Qt
License:    GPLv2
URL:        https://you-ra.info
Source0:    %{name}-%{version}.tar.bz2

Requires:   sailfishsilica-qt5 >= 0.10.9
#Requires:   ffmpeg
BuildRequires:  pkgconfig(auroraapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  desktop-file-utils
BuildRequires:  cmake
BuildRequires:  ffmpeg-devel
BuildRequires:  libvncserver-devel
BuildRequires:  lipstick2vnc
%description
Screen recorder application

%prep
%setup -q -n %{name}-%{version}
#cd FFmpegBuild && cmake .

%build
%cmake
cmake --build . -v

%install
rm -rf %{buildroot}
DESTDIR=%{buildroot} cmake --build . --target install
desktop-file-install --delete-original         --dir %{buildroot}%{_datadir}/applications                %{buildroot}%{_datadir}/applications/*.desktop

mkdir -p %{buildroot}%{_datadir}/%{name}/lib/
mkdir -p %{buildroot}/usr/libexec/%{name}/
cp /usr/lib/libavcodec.so.* %{buildroot}%{_datadir}/%{name}/lib/
cp /usr/lib/libavformat.so.* %{buildroot}%{_datadir}/%{name}/lib/
cp /usr/lib/libavutil.so.* %{buildroot}%{_datadir}/%{name}/lib/
cp /usr/lib/libswscale.so.* %{buildroot}%{_datadir}/%{name}/lib/
cp /usr/lib/libvncclient.so.* %{buildroot}%{_datadir}/%{name}/lib/
cp /usr/lib/libswresample.so.* %{buildroot}%{_datadir}/%{name}/lib/

cp /usr/lib/libvncserver.so.* %{buildroot}/usr/libexec/%{name}/
cp /usr/bin/lipstick2vnc %{buildroot}/usr/libexec/%{name}/

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
/usr/libexec/%{name}
%defattr(644,root,root,-)
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
