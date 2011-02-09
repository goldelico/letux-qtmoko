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
cd $qtmoko_dir/etc/themes/faenqo/ && dpkg-buildpackage
mv $qtmoko_dir/etc/themes/qtmoko-theme-faenqo_*_all.deb $outdir

echo "============================================= Packages should be now in $outdir"