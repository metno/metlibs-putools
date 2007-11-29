#! /bin/bash
#
# $Id: bootstrap.sh 536 2007-11-29 13:38:32Z martinr $
#

# Get version and check if --install can be used.
autoreconf=$(which autoreconf)
value=$(autoconf --version  | head -n 1 | sed -e "s/.* //")

minor=${value#*.}
major=${value%.*}

if [ $major -le 2 ] && [ $minor -le 59 ]; then
    install=""
else
    install="--install"
fi


if [ -n "${install}" ] && [ -n "${autoreconf}" ] ; then
    set -x
    $autoreconf
else
    set -x
    aclocal -I m4
    libtoolize --force --copy
    autoheader
    automake --add-missing --copy
    autoconf
fi
