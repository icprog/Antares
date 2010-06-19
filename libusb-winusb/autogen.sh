#!/bin/sh
# use glibtoolize if it is available (darwin)
(glibtoolize --version) < /dev/null > /dev/null 2>&1 && LIBTOOLIZE=glibtoolize || LIBTOOLIZE=libtoolize

$LIBTOOLIZE --copy --force || exit 1
# If available, apply libtool's NLS patch to set locale to C always.
# Prevents an issue when compiling shared libs with MinGW on Chinese locale.
# see: http://lists.gnu.org/archive/html/bug-libtool/2010-03/msg00012.html
type -P patch &>/dev/null && { if [ -e "libtool-nls.diff" ]; then patch -p1 -Nl -i libtool-nls.diff &>/dev/null; fi; }
#
aclocal || exit 1
autoheader || exit 1
autoconf || exit 1
automake -a -c || exit 1
./configure --enable-debug-log --enable-examples-build $*
