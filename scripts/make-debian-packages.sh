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

echo "====================================== Building qtmoko-keyboard-russian-abc.deb"
cd $qtmoko_dir/src/3rdparty/plugins/inputmethods/keyboard-russian-abc && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/plugins/inputmethods/qtmoko-keyboard-russian-abc_*-* $outdir

echo "====================================== Building qtmoko-keyboard-russian-abc.deb"
cd $qtmoko_dir/src/3rdparty/plugins/inputmethods/keyboard-skin-silver && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/plugins/inputmethods/qtmoko-keyboard-skin-silver_*-* $outdir

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
cd $qtmoko_dir/src/3rdparty/plugins/codecs/codecs-package && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/plugins/codecs/qtmoko-codecs_*-* $outdir

echo "==================================================== Building qtmoko-qgcide.deb"
cd $qtmoko_dir/src/3rdparty/applications/qgcide && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qgcide_*-* $outdir

echo "=============================================== Building qtmoko-melodiq.deb"
cd $qtmoko_dir/src/3rdparty/applications/melodiq && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-melodiq_*-* $outdir

echo "=============================================== Building qtmoko-qtpedometer.deb"
cd $qtmoko_dir/src/3rdparty/applications/qtpedometer && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qtpedometer_*-* $outdir

echo "================================================== Building qtmoko-eyepiece.deb"
cd $qtmoko_dir/src/3rdparty/applications/eyepiece && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-eyepiece_*-* $outdir

echo "==================================================== Building qtmoko-gqsync.deb"
cd $qtmoko_dir/src/3rdparty/applications/gqsync && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-gqsync_*-* $outdir

echo "=============================================== Building qtmoko-greensudoku.deb"
cd $qtmoko_dir/src/3rdparty/games/greensudoku && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/games/qtmoko-greensudoku_*-* $outdir

echo "================================================= Building qtmoko-keypebble.deb"
cd $qtmoko_dir/src/3rdparty/applications/keypebble && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-keypebble_*-* $outdir

echo "================================================== Building qtmoko-phonetiq.deb"
cd $qtmoko_dir/src/3rdparty/applications/phonetiq && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-phonetiq_*-* $outdir

echo "=============================================== Building qtmoko-qmokoplayer.deb"
cd $qtmoko_dir/src/3rdparty/applications/qmokoplayer && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qmokoplayer_*-* $outdir

echo "================================================= Building qtmoko-solitaire.deb"
cd $qtmoko_dir/src/3rdparty/games/qsolitaire && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/games/qtmoko-solitaire_*-* $outdir

echo "================================================= Building qtmoko-qtopiagps.deb"
cd $qtmoko_dir/src/3rdparty/applications/qtopiagps && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qtopiagps_*-* $outdir

echo "================================================== Building qtmoko-qweather.deb"
cd $qtmoko_dir/src/3rdparty/applications/qweather && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qweather_*-* $outdir

echo "================================================== Building qtmoko-spectemu.deb"
cd $qtmoko_dir/src/3rdparty/applications/spectemu && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-spectemu_*-* $outdir

echo "==================================================== Building qtmoko-gqsync.deb"
cd $qtmoko_dir/src/3rdparty/applications/gqsync && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-gqsync_*-* $outdir

echo "==================================================== Building qtmoko-mqutim.deb"
cd $qtmoko_dir/src/3rdparty/applications/mqutim && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-mqutim_*-* $outdir

echo "=================================================== Building qtmoko-noxchat.deb"
cd $qtmoko_dir/src/3rdparty/applications/noxchat && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-noxchat_*-* $outdir

echo "================================================ Building qtmoko-qalsamixer.deb"
cd $qtmoko_dir/src/3rdparty/applications/qalsamixer && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qalsamixer_*-* $outdir

echo "================================================== Building qtmoko-dictopia.deb"
cd $qtmoko_dir/src/3rdparty/applications/qdictopia && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-dictopia_*-* $outdir

echo "========================================== Building qtmoko-qgoogletranslate.deb"
cd $qtmoko_dir/src/3rdparty/applications/qgoogletranslate && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qgoogletranslate_*-* $outdir

echo "================================================== Building qtmoko-qneoroid.deb"
cd $qtmoko_dir/src/3rdparty/applications/qneoroid && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qneoroid_*-* $outdir

echo "================================================== Building qtmoko-qtbackup.deb"
cd $qtmoko_dir/src/3rdparty/applications/qtbackup && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qtbackup_*-* $outdir

echo "=============================================== Building qtmoko-qtpedometer.deb"
cd $qtmoko_dir/src/3rdparty/applications/qtpedometer && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-qtpedometer_*-* $outdir

echo "=================================================== Building qtmoko-shopper.deb"
cd $qtmoko_dir/src/3rdparty/applications/shopper && dpkg-buildpackage -tc
mv $qtmoko_dir/src/3rdparty/applications/qtmoko-shopper_*-* $outdir

echo "============================================= Packages should be now in $outdir"