#!/bin/bash
# build the qtmoko.deb packages from sources
#
# run this script on a Debian Wheezy ARM machine (virtual or real)
#
# 1. uses git to fetch the sources
# 2. use dpkg-buildpackage to run the complete build process

[ "$VERSION" ] || export VERSION=58

SOURCE=/qtmoko-src

echo "mdeb-qtmoko: updating Debian $(cat /etc/debian_version) within Qemu"

apt-get update -y
apt-get upgrade -y

echo install developer toolset
apt-get install -y git dpkg-dev build-essential

echo "mdeb-qtmoko: getting source tree"
if [ -d "$SOURCE/.git" ]
then
	(cd "$SOURCE" && git fetch) || exit 1
else
	rm -rf "$SOURCE/.git"
	# git clone https://github.com/radekp/qtmoko.git
	git clone git://git.goldelico.com/gta04-qtmoko.git "$SOURCE"
fi

cd "$SOURCE" || exit 1

echo "mdeb-qtmoko: checkout v${VERSION}"
git checkout --force v${VERSION}	# switch to branch
git pull	# update from upstream

echo "mdeb-qtmoko: checkout submodules"
# ...and checkout git submodules as it's necessary to place qt sources into qtmoko/qtopiacore/qt/:
git submodule update --force --init
echo "mdeb-qtmoko: dpkg-buildpackage"
echo '3.0 (native)' > debian/source/format # see http://code.google.com/p/pymds/issues/detail?id=1

echo "mdeb-qtmoko: install specific build dependencies"
depends=$(fgrep -h Build-Depends debian/control* | sed "s/^Build-Depends: //g" | sed "s/@.*@//g" | sed "s/([^)]*)//g" | sed "s/,/ /g")
# substitutes
depends=$(echo $depends | sed 's/libtiff4-dev /libtiff5-dev /g')
depends=$(echo $depends | sed 's/libts-dev //g')
echo "mdeb-qtmoko: dependencies" $depends
apt-get install -y $depends

echo "mdeb-qtmoko: dpkg-buildpackage"
# build packages but don't clean (so that a rebuild is faster)
# use debuild -nc --prepend-path=/usr/lib/ccache --preserve-envvars=CCACHE_* instead?

dpkg-buildpackage -nc

echo "mdeb-qtmoko: dpkg-buildpackage done"

ls -l
