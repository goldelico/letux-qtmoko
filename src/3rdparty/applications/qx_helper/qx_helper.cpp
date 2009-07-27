#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/vt.h>
#include <linux/kd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

int main(int argc, char **argv)
{
	int  LinuxConsoleFd;
	
	fprintf(stderr, "QX helper started\n");

    if ((LinuxConsoleFd = open("/dev/tty7", O_RDWR|O_NDELAY, 0)) < 0)
    {
		perror("QX helper: Cannot open /dev/tty7");
		return 0;
    }
    
    if (ioctl(LinuxConsoleFd, VT_ACTIVATE, 7) != 0)
    {
		fprintf(stderr, "QX helper: VT_ACTIVATE failed\n");
    }
	fprintf(stderr, "QX helper: calling VT_WAITACTIVE for /dev/tty7\n");
    if (ioctl(LinuxConsoleFd, VT_WAITACTIVE, 7) != 0)
    {
		fprintf(stderr, "QX helper: VT_WAITACTIVE failed\n");
    }
    fprintf(stderr, "QX helper: VT_WAITACTIVE done\n");
    close(LinuxConsoleFd);
	sleep(180);
}
