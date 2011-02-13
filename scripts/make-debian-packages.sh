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

echo "==================================================== Building qtmoko-codecs.deb"
cd $qtmoko_dir/qtmoko-apps/codecs && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-codecs_*-* $outdir

echo "==================================================== Building qtmoko-qgcide.deb"
cd $qtmoko_dir/qtmoko-apps/qgcide && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qgcide_*-* $outdir

echo "=============================================== Building qtmoko-qtpedometer.deb"
cd $qtmoko_dir/qtmoko-apps/qtpedometer && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qtpedometer_*-* $outdir

# echo "================================================== Building qtmoko-eyepiece.deb"
# cd $qtmoko_dir/qtmoko-apps/eyepiece && dpkg-buildpackage -tc
# mv $qtmoko_dir/qtmoko-apps/qtmoko-eyepiece_*-* $outdir

echo "==================================================== Building qtmoko-gqsync.deb"
cd $qtmoko_dir/qtmoko-apps/gqsync && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-gqsync_*-* $outdir

echo "=============================================== Building qtmoko-greensudoku.deb"
cd $qtmoko_dir/qtmoko-apps/greensudoku && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-greensudoku_*-* $outdir

echo "================================================= Building qtmoko-keypebble.deb"
cd $qtmoko_dir/qtmoko-apps/keypebble && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-keypebble_*-* $outdir

echo "================================================== Building qtmoko-phonetiq.deb"
cd $qtmoko_dir/qtmoko-apps/phonetiq && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-phonetiq_*-* $outdir

echo "=============================================== Building qtmoko-qmokoplayer.deb"
cd $qtmoko_dir/qtmoko-apps/qmokoplayer && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qmokoplayer_*-* $outdir

echo "================================================= Building qtmoko-solitaire.deb"
cd $qtmoko_dir/qtmoko-apps/qsolitaire && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-solitaire_*-* $outdir

echo "================================================= Building qtmoko-qtopiagps.deb"
cd $qtmoko_dir/qtmoko-apps/qtopiagps && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qtopiagps_*-* $outdir

echo "================================================== Building qtmoko-qweather.deb"
cd $qtmoko_dir/qtmoko-apps/qweather && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qweather_*-* $outdir

echo "================================================== Building qtmoko-spectemu.deb"
cd $qtmoko_dir/qtmoko-apps/spectemu && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-spectemu_*-* $outdir

echo "==================================================== Building qtmoko-gqsync.deb"
cd $qtmoko_dir/qtmoko-apps/gqsync && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-gqsync_*-* $outdir

echo "==================================================== Building qtmoko-mqutim.deb"
cd $qtmoko_dir/qtmoko-apps/mqutim && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-mqutim_*-* $outdir

echo "=================================================== Building qtmoko-noxchat.deb"
cd $qtmoko_dir/qtmoko-apps/noxchat && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-noxchat_*-* $outdir

echo "================================================ Building qtmoko-qalsamixer.deb"
cd $qtmoko_dir/qtmoko-apps/qalsamixer && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qalsamixer_*-* $outdir

echo "================================================== Building qtmoko-dictopia.deb"
cd $qtmoko_dir/qtmoko-apps/qdictopia && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-dictopia_*-* $outdir

echo "========================================== Building qtmoko-qgoogletranslate.deb"
cd $qtmoko_dir/qtmoko-apps/qgoogletranslate && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qgoogletranslate_*-* $outdir

echo "================================================== Building qtmoko-qneoroid.deb"
cd $qtmoko_dir/qtmoko-apps/qneoroid && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qneoroid_*-* $outdir

echo "================================================== Building qtmoko-qtbackup.deb"
cd $qtmoko_dir/qtmoko-apps/qtbackup && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qtbackup_*-* $outdir

echo "=============================================== Building qtmoko-qtpedometer.deb"
cd $qtmoko_dir/qtmoko-apps/qtpedometer && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-qtpedometer_*-* $outdir

echo "=================================================== Building qtmoko-shopper.deb"
cd $qtmoko_dir/qtmoko-apps/shopper && dpkg-buildpackage -tc
mv $qtmoko_dir/qtmoko-apps/qtmoko-shopper_*-* $outdir

echo "============================================= Packages should be now in $outdir"