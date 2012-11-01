# This script install debian squeeze chroot with all dependencies for QtMoko

set -o verbose

# Unmounting chroot dirs
umount ../qtmoko-chroot/proc
umount ../qtmoko-chroot/sys
umount ../qtmoko-chroot/dev
umount ../qtmoko-chroot/root/qte/qtmoko
umount ../qtmoko-chroot/root/qte/build
umount ../qtmoko-chroot

# Done