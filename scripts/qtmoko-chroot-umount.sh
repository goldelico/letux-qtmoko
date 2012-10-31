# This script install debian squeeze chroot with all dependencies for QtMoko

echo "Unmounting chroot dirs"

umount ../build-chroot/proc
umount ../build-chroot/sys
umount ../build-chroot/dev
umount ../build-chroot/root/qte/qtmoko
umount ../build-chroot

echo "Done"
