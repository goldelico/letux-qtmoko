#!/bin/sh

# output dir
outdir=$PWD/debian_packages
rm -rf $outdir
mkdir $outdir

# determine path to this script if it's symlink
script_path=$0
if [ -L "$0" ]
then
    script_path=$( readlink "$0" )
fi

echo $script_path

script_dir=${script_path%/*}
echo $script_dir

qtmoko_dir=${script_dir%/*}
echo $qtmoko_dir

echo "============================================== Building qtmoko-theme-faenqo.deb"
cd $qtmoko_dir/etc/themes/faenqo/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-faenqo_*-* $outdir

echo "=========================================== Building qtmoko-theme-asthromod.deb"
cd $qtmoko_dir/etc/themes/asthromod/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-asthromod_*-* $outdir

echo "============================================= Building qtmoko-theme-classic.deb"
cd $qtmoko_dir/etc/themes/classic/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-classic_*-* $outdir

echo "=============================================== Building qtmoko-theme-crisp.deb"
cd $qtmoko_dir/etc/themes/crisp/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-crisp_*-* $outdir

echo "=========================================== Building qtmoko-theme-deskphone.deb"
cd $qtmoko_dir/etc/themes/deskphone/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-deskphone_*-* $outdir

echo "============================================ Building qtmoko-theme-finximod.deb"
cd $qtmoko_dir/etc/themes/finximod/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-finximod_*-* $outdir

echo "=========================================== Building qtmoko-theme-home-wvga.deb"
cd $qtmoko_dir/etc/themes/home_wvga/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-home-wvga_*-* $outdir

echo "============================================== Building qtmoko-theme-qtopia.deb"
cd $qtmoko_dir/etc/themes/qtopia/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-qtopia_*-* $outdir

echo "=============================================== Building qtmoko-theme-smart.deb"
cd $qtmoko_dir/etc/themes/smart/ && dpkg-buildpackage -tc
mv $qtmoko_dir/etc/themes/qtmoko-theme-smart_*-* $outdir

echo "============================================= Packages should be now in $outdir"