#
# Regular cron jobs for the qtmoko package
#
0 4	* * *	root	[ -x /usr/bin/qtmoko_maintenance ] && /usr/bin/qtmoko_maintenance
