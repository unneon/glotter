#!/bin/bash
function run {
	echo "$@"
	$@
}
function ask {
	read -p $'\e[1;34m'"$1"$': \e[0m' $2
}
function debwrite {
	echo $1 >> $DIRNAME/DEBIAN/control
}

ask "Version and revision" V
DIRNAME="glotter_$V"
run mkdir -p $DIRNAME
run mkdir -p $DIRNAME/usr
run mkdir -p $DIRNAME/usr/include
run mkdir -p $DIRNAME/usr/share
run mkdir -p $DIRNAME/usr/share/glotter
run mkdir -p $DIRNAME/DEBIAN
run cp include/glotter.hpp $DIRNAME/usr/include
run cp web/* $DIRNAME/usr/share/glotter

run > $DIRNAME/DEBIAN/control
run debwrite "Package: glotter"
run debwrite "Version: $V"
ask "Package section" PKG_SECT
run debwrite "Section: $PKG_SECT"
ask "Architecture" PKG_ARCH
run debwrite "Priority: optional"
run debwrite "Architecture: $PKG_ARCH"
ask "Name of uWS package" PKG_DEP_UWS
run debwrite "Depends: libssl-dev, zlib1g-dev, $PKG_DEP_UWS"
ask "Maintainer(and email)" PKG_MAINTAINER
run debwrite "Maintainer: $PKG_MAINTAINER"
run debwrite "Description: graph algorithms debugging visualization tool"
