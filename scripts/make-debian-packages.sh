#!/bin/sh

# output dir
outdir=$PWD/debian_packages
rm -rf $outdir
mkdir $outdir

## FIXME: does not correctly handle relative paths

# determine path to this script if it's symlink
script_path=$0
if [ -L "$0" ]
then
    script_path=$( readlink "$0" )
fi

echo PWD=$PWD
echo script_path=$script_path

script_dir=${script_path%/*}
echo script_dir=$script_dir

qtmoko_dir=${script_dir%/*}
echo qtmoko_dir=$qtmoko_dir

PACKAGES="$PACKAGES src/3rdparty/plugins/inputmethods/keyboard-russian-abc"
PACKAGES="$PACKAGES src/3rdparty/plugins/inputmethods/keyboard-skin-silver"
PACKAGES="$PACKAGES etc/themes/faenqo"
PACKAGES="$PACKAGES etc/themes/asthromod"
PACKAGES="$PACKAGES etc/themes/classic"
PACKAGES="$PACKAGES etc/themes/crisp"
PACKAGES="$PACKAGES etc/themes/deskphone"
PACKAGES="$PACKAGES etc/themes/finximod"
PACKAGES="$PACKAGES etc/themes/home_wvga"
PACKAGES="$PACKAGES etc/themes/qtopia"
PACKAGES="$PACKAGES etc/themes/smart"
# FIXME: aborts with cp: cannot stat `../../../../../../build/sdk/lib/libmad.so*': No such file or directory
# PACKAGES="$PACKAGES src/3rdparty/plugins/codecs/libmad"
# PACKAGES="$PACKAGES src/3rdparty/plugins/codecs/codecs-package"
PACKAGES="$PACKAGES src/3rdparty/applications/qgcide"
PACKAGES="$PACKAGES src/3rdparty/applications/melodiq"
PACKAGES="$PACKAGES src/3rdparty/applications/qtpedometer"
PACKAGES="$PACKAGES src/3rdparty/applications/eyepiece"
PACKAGES="$PACKAGES src/3rdparty/applications/gqsync"
PACKAGES="$PACKAGES src/3rdparty/games/greensudoku"
PACKAGES="$PACKAGES src/3rdparty/games/cutemaze"
PACKAGES="$PACKAGES src/3rdparty/games/gottet"
PACKAGES="$PACKAGES src/3rdparty/games/qnetwalk"
PACKAGES="$PACKAGES src/3rdparty/games/qsamegame"
PACKAGES="$PACKAGES src/3rdparty/applications/keypebble"
PACKAGES="$PACKAGES src/3rdparty/applications/phonetiq"
PACKAGES="$PACKAGES src/3rdparty/applications/qmokoplayer"
PACKAGES="$PACKAGES src/3rdparty/games/qsolitaire"
PACKAGES="$PACKAGES src/3rdparty/applications/qtopiagps"
PACKAGES="$PACKAGES src/3rdparty/applications/qweather"
PACKAGES="$PACKAGES src/3rdparty/applications/spectemu"
PACKAGES="$PACKAGES src/3rdparty/applications/gqsync"
PACKAGES="$PACKAGES src/3rdparty/applications/mqutim"
PACKAGES="$PACKAGES src/3rdparty/applications/noxchat"
PACKAGES="$PACKAGES src/3rdparty/applications/qalsamixer"
PACKAGES="$PACKAGES src/3rdparty/applications/qdictopia"
PACKAGES="$PACKAGES src/3rdparty/applications/qgoogletranslate"
PACKAGES="$PACKAGES src/3rdparty/applications/qneoroid"
PACKAGES="$PACKAGES src/3rdparty/applications/qtbackup"
PACKAGES="$PACKAGES src/3rdparty/applications/qtpedometer"
PACKAGES="$PACKAGES src/3rdparty/applications/shopper"

for i in $PACKAGES
do
	(
	echo "=================================================== Building qtmoko-$(basename $i).deb"
	cd $qtmoko_dir/$i && pwd &&
# FIXME: does not find ../../../../../build/bin/qbuild
	dpkg-buildpackage -tc &&
	mv $qtmoko_dir/$(dirname $i)/qtmoko-$(basename $i)_*-* $outdir
	)
done

echo "============================================= Packages should be now in $outdir"
