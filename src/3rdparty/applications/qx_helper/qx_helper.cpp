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
	int fd;
    const char termctl[] = "\033[9;15]\033[?33h\033[?25h\033[?0c";
	
	//fprintf(stderr, "QX helper started\n");

    if ((fd = open("/dev/tty4", O_RDWR|O_NDELAY, 0)) < 0)
    {
		perror("QX helper: Cannot open /dev/tty4");
		return 0;
    }
    
    if (ioctl(fd, VT_ACTIVATE, 4) != 0)
    {
		fprintf(stderr, "QX helper: VT_ACTIVATE failed\n");
    }
	//fprintf(stderr, "QX helper: calling VT_WAITACTIVE for /dev/tty4\n");
    if (ioctl(fd, VT_WAITACTIVE, 4) != 0)
    {
		fprintf(stderr, "QX helper: VT_WAITACTIVE failed\n");
    }
    
    write(fd, termctl, sizeof(termctl));
    
    //fprintf(stderr, "QX helper: VT_WAITACTIVE done\n");
    close(fd);
	sleep(180);		// wait until qtopia starts
}
