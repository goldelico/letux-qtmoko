#!/bin/bash
# make qtmoko packages from sources
#
# run this script on a Debian Wheezy ARM machine (virtual or real)
#
# 1. uses git to fetch the sources
# 2. use dpkg-buildpackage to run the complete build process

[ "$VERSION" ] || export VERSION=58

echo "mbuild: updating Debian $(cat /etc/debian_version) within Qemu"

apt-get update -y
apt-get upgrade -y

echo install developer toolset
apt-get install -y git dpkg-dev build-essential

echo "mbuild: getting source tree"
# here we should simply cd to a shared directory with the already checked out git tree...
if [ -d qtmoko ]
then
	(cd qtmoko; git fetch)
else
	git clone https://github.com/radekp/qtmoko.git
fi

cd qtmoko || exit 1

echo "mbuild: checkout v${VERSION}"
git checkout -f v${VERSION}
# Please note that releases are made from translations branch so that we have
# all languages.

echo "mbuild: checkout submodules"
# ...and checkout git submodules as it's necessary to place qt sources into qtmoko/qtopiacore/qt/:
git submodule update --init
echo "mbuild: dpkg-buildpackage"
echo '3.0 (native)' > debian/source/format # see http://code.google.com/p/pymds/issues/detail?id=1

echo "mbuild: install specific build dependencies"
depends=$(cat debian/control* | fgrep Build-Depends | sed "s/^Build-Depends: //g")
depends=$(echo $depends | sed "s/[() ]//g" | tr "," " ")
apt-get install -y $depends

echo "mbuild: dpkg-buildpackage"
# build packages but don't clean (so that a rebuild is faster)
# use debuild -nc --prepend-path=/usr/lib/ccache --preserve-envvars=CCACHE_* instead?

dpkg-buildpackage -nc

echo "mbuild: dpkg-buildpackage done"

ls -l
