/*  accelerometers.cpp
 *
 *  Accelerometer stuff
 *
 *  (c) 2009-2010 Anton Olkhovik <ant007h@gmail.com>
 *
 *  This file is part of QtMaze (port of Mokomaze) - labyrinth game.
 *
 *  QtMaze is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  QtMaze is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with QtMaze.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <linux/input.h>
#include <linux/joystick.h>

#include <pthread.h>
#include "accelerometers.h"
#include "types.h"

pthread_t thread;
int finished=0;
double acx=0,acy=0,acz=0;

#define JS_DEVICE_NEO "/dev/input/js1"
#define JS_DEVICE_PC  "/dev/input/js0"
#define MAX_AXIS_NEO  1024.0 //1000.0
#define MAX_AXIS_PC  64000.0 //32767.0
double max_axis = MAX_AXIS_NEO;
int pc_mode=0;

#define GET_DATA_INTERVAL 2000

/* The accelerometer work thread */
void* accel_work(void *data)
{
        int *finished = (int*)data;

        int fd;
        if ((fd = open(JS_DEVICE_NEO, O_RDONLY)) < 0)
        {
                fprintf(stderr, "Accelerometer: error opening file " JS_DEVICE_NEO "\n");
                pc_mode = 1;
                max_axis = MAX_AXIS_PC;
                if ((fd = open(JS_DEVICE_PC, O_RDONLY)) < 0)
                {
                        fprintf(stderr, "Accelerometer: error opening file " JS_DEVICE_PC "\n");
                        return NULL;
                }
        }

        unsigned char axes = 2;
        //unsigned char buttons = 2;
        ioctl(fd, JSIOCGAXES, &axes);
        //ioctl(fd, JSIOCGBUTTONS, &buttons);

        int *axis;
        //char *button;
        struct js_event js;

        axis = (int*)calloc(axes, sizeof(int));
        //button = (char*)calloc(buttons, sizeof(char));

        while ( !(*finished) )
        {            
                size_t rval;
                fd_set fds;
                struct timeval t;

                FD_ZERO(&fds);
                FD_SET(fd, &fds);
                t.tv_sec = 0;
                t.tv_usec = 0;
                select(1+fd, &fds, NULL, NULL, &t);

                if ( FD_ISSET(fd, &fds) )
                {
                        rval = read(fd, &js, sizeof(js));
                        if ( rval != sizeof(js) )
                        {
                                fprintf(stderr, "Accelerometer: error reading\n");
                                return NULL;
                        }

                        switch(js.type & ~JS_EVENT_INIT)
                        {
                        //case JS_EVENT_BUTTON:
                        //        button[js.number] = js.value;
                        //        break;
                        case JS_EVENT_AXIS:
                                axis[js.number] = js.value;
                                break;
                        }

                        acx = axis[0] / max_axis;
                        acy = axis[1] / max_axis;
                        acz = axis[2] / max_axis;

                        if (acx<-1) acx=-1;
                        if (acy<-1) acy=-1;
                        if (acz<-1) acz=-1;

                        if (acx>1) acx=1;
                        if (acy>1) acy=1;
                        if (acz>1) acz=1;
                }
                else
                {
                        /* No data */
                }

                usleep(GET_DATA_INTERVAL);
        }

        close(fd);

	return NULL;
}

void accelerometer_start()
{
	finished = 0;
        pthread_create( &thread, NULL, accel_work, (void*)(&finished));
}

void accelerometer_stop()
{
	finished = 1;
	pthread_join(thread, NULL);
}

double getacx()
{
	return acx;
}
double getacy()
{
        return pc_mode?-acy:acy;
}
double getacz()
{
	return acz;
}
