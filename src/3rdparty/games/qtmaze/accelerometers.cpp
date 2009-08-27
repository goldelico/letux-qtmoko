/*  accelerometers.cpp
 *
 *  Accelerometer stuff
 *
 *  (c) 2009 Anton Olkhovik <ant007h@gmail.com>
 *
 *  Based on original accelerometers.c from OpenMooCow - accelerometer moobox simulator.
 *  (c) 2008 Thomas White <taw27@srcf.ucam.org>
 *  (c) 2008 Joachim Breitner <mail@joachim-breitner.de>
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>

#include <pthread.h>
#include "accelerometers.h"
#include "types.h"

#include <sys/utsname.h>
#include <string.h>

pthread_t thread;
int finished=0;
double acx=0,acy=0,acz=0;

#define GET_DATA_INTERVAL 12500 //25000

typedef struct {

	int		fd;

	AccelType	type;

	int		x;
	int		y;
	int		z;

	/* Temporary values for use during reading */
	int		lx;
	int		ly;
	int		lz;

	/* Current state (driver dependent) */
	int		state;
	signed int	old_threshold;
	int     	threshold_changed;

} AccelHandle;

struct input_event {
	struct timeval time;
	uint16_t type;
	uint16_t code;
	int32_t value;
};
#define EV_SYN (0x00)
#define EV_REL (0x02)
#define EV_ABS (0x03)
#define SYN_REPORT (0x00)
#define REL_X (0x00)
#define REL_Y (0x01)
#define REL_Z (0x02)
#define ABS_X (0x00)
#define ABS_Y (0x01)
#define ABS_Z (0x02)

/* Try to open the threshold sysfs file for Freerunner's accelerometers.
 * Try methods for both old and new kernels */
static FILE *accelerometer_freerunner_open_threshold(const char *mode)
{
	FILE *fh;

	/* Try 2.6.24 method */
	fh = fopen("/sys/devices/platform/lis302dl.2/threshold", mode);
	if ( fh != NULL ) {
		return fh;
	}

	/* Try 2.6.28+ method */
	fh = fopen("/sys/class/i2c-adapter/i2c-0/0-0073/lis302dl.2/threshold", mode);
	if ( fh != NULL ) {
		return fh;
	}

	/* Try 2.6.29+ method */
	fh = fopen("/sys/class/i2c-adapter/i2c-0/0-0073/spi_s3c24xx_gpio.0/spi3.1/threshold", mode);
	if ( fh != NULL ) {
		return fh;
	}

	return NULL;
}

static void accelerometer_freerunner_try_threshold(AccelHandle *accel)
{

        FILE *fh;
	int rval;

        accel->old_threshold = -1;
        accel->threshold_changed = 0;

	/* Save old threshold value */
	fh = accelerometer_freerunner_open_threshold("r");
        if ( fh == NULL )
        {
            fprintf(stderr, "Accelerometer: can't open threshold file for reading.\n");
            return;
        }
        signed int trh = -1;
        rval = fscanf(fh, "%i", &trh);
	if ( rval != 1 )
        {
            fprintf(stderr, "Accelerometer: can't read threshold value.\n");
	}
        else
        {
            printf("Accelerometer: current threshold value is %i\n", trh);
            accel->old_threshold = trh;
        }
        fclose(fh);

        if (trh == 0)
        {
            printf("Accelerometer: threshold is not required to be changed.\n");
            return;
        }

	/* Set threshold to zero */
	fh = accelerometer_freerunner_open_threshold("w");
	if ( fh == NULL )
        {
            fprintf(stderr, "Accelerometer: can't open threshold file for writing.\n");
            return;
	}
	fprintf(fh, "0\n");
	fclose(fh);
        printf("Accelerometer: threshold disabled successfully.\n");
        accel->threshold_changed = 1;

}

static void accelerometer_freerunner_try_restore_threshold(AccelHandle *accel)
{

	FILE *fh;
	int rval;
	int new_threshold = 0;

	if ( (!(accel->threshold_changed)) ||
               (accel->old_threshold < 0) )
        {
		printf("Accelerometer: threshold is not required to be restored.\n");
                return;
	}

        printf("Accelerometer: trying to restore old threshold value.\n");

	fh = accelerometer_freerunner_open_threshold("r");
	if ( fh == NULL ) {
		fprintf(stderr, "Accelerometer: can't open threshold file for reading.\n");
		return;
	}
	rval = fscanf(fh, "%i", &new_threshold);
	if ( rval != 1 ) {
		fprintf(stderr, "Accelerometer: can't read new threshold\n");
                new_threshold = 0;
	}
	fclose(fh);

	/* Restore only if it hasn't been altered externally */
	if ( new_threshold != 0 ) {
		printf("Accelerometer: threshold has been altered externally - not restoring.\n");
		return;
	}

	/* Set it back to the old value */
	fh = accelerometer_freerunner_open_threshold("w");
	if ( fh == NULL ) {
		fprintf(stderr, "Accelerometer: can't open threshold file for writing.\n");
		return;
	}
	fprintf(fh, "%i\n", accel->old_threshold);
	fclose(fh);
	printf("Accelerometer: threshold restored successfully.\n");

}

AccelHandle *accelerometer_open()
{
        AccelHandle *accel;

        /* Initialise accelerometer data structure */
        accel = (AccelHandle*)malloc(sizeof(AccelHandle));
        if ( accel == NULL ) return NULL;
        accel->x = 0;
        accel->y = 0;
        accel->z = 0;
        accel->lx = 0;
        accel->ly = 0;
        accel->lz = 0;
        accel->state = 0;
        accel->type = ACCEL_UNKNOWN;

        int cant_open = 0;
        struct stat st;

        char freerunner_file_default[] = "/dev/input/event3";
        char freerunner_file_2630[]    = "/dev/input/event4";
        char *freerunner_file = freerunner_file_default;

        struct utsname uts;
        if ( uname(&uts) < 0 )
            printf("Accelerometer: unable to get host information\n");
        else
            if ( strstr(uts.release, "2.6.30") )
            {
                printf("Accelerometer: Linux kernel 2.6.30 detected\n");
                freerunner_file = freerunner_file_2630;
            }

        if (stat(freerunner_file, &st) == 0)
        {
                accel->fd = open(freerunner_file, O_RDONLY, O_NONBLOCK); //, O_RDONLY, 0);
                if ( accel->fd != -1 )
                {
                        accel->type = ACCEL_FREERUNNER;
                        printf("Accelerometer: Neo Freerunner detected\n");
                        accelerometer_freerunner_try_threshold(accel);
                        return accel;
                }
                else
                {
                        fprintf(stderr, "Accelerometer: Neo Freerunner accelerometer file detected, but can't be opened. " \
                                        "Check access permissions\n");
                        cant_open = 1;
                }
        }
        else
        {
            fprintf(stderr, "Accelerometer: Neo Freerunner accelerometer file not found\n");
            cant_open = 1;
        }

        fprintf(stderr, "Accelerometer: no input device available\n");
        return accel;
}

static void accelerometer_shutdown(AccelHandle *accel)
{
	if ( accel->type == ACCEL_FREERUNNER ) {
		accelerometer_freerunner_try_restore_threshold(accel);
	}
}

int accelerometer_moo_freerunner(AccelHandle *accel)
{
	struct input_event ev;
	size_t rval;
	fd_set fds;
	struct timeval t;

	FD_ZERO(&fds);
	FD_SET(accel->fd, &fds);
	t.tv_sec = 0;
	t.tv_usec = 0;
	select(1+accel->fd, &fds, NULL, NULL, &t);

	if ( FD_ISSET(accel->fd, &fds) ) {
            
		rval = read(accel->fd, &ev, sizeof(ev));
		if ( rval != sizeof(ev) ) {
			fprintf(stderr, "Accelerometer: couldn't read data\n");
			return 0;
		}

	} else {
		return 0;	/* No data */
	}

	if ( ev.type == EV_REL ) {
		if ( ev.code == REL_X ) {
			accel->lx = ev.value;
		}
		if ( ev.code == REL_Y ) {
			accel->ly = ev.value;
		}
		if ( ev.code == REL_Z ) {
			accel->lz = ev.value;
		}
	}

	if ( ev.type == EV_ABS ) {
		if ( ev.code == ABS_X ) {
			accel->lx = ev.value;
		}
		if ( ev.code == ABS_Y ) {
			accel->ly = ev.value;
		}
		if ( ev.code == ABS_Z ) {
			accel->lz = ev.value;
		}
	}

	if ( ev.type == EV_SYN ) {
		if ( ev.code == SYN_REPORT ) {
			accel->x = accel->lx;
			accel->y = accel->ly;
			accel->z = accel->lz;
		}
	}

	acx = accel->x/1000.0;
	acy = accel->y/1000.0;
	acz = accel->z/1000.0;

        if (acx<-1) acx=-1; if (acx>1) acx=1;
        if (acy<-1) acy=-1; if (acy>1) acy=1;
        //if (acz<-1) acz=-1; if (acz>1) acz=1;
	
	return 0;
}

int accelerometer_moo(AccelHandle *accel)
{
	switch ( accel->type ) {
		case ACCEL_UNKNOWN : {
			return 0;
		}
		case ACCEL_FREERUNNER : {
			return accelerometer_moo_freerunner(accel);
		}
		/* Add other types here. */
	}

	return 0;
}

/* The accelerometer work thread */
void* accel_work(void *data)
{
	AccelHandle *accel;
        int *finished = (int*)data;

	accel = accelerometer_open();
	while ( !(*finished) ) {
                accelerometer_moo(accel);
                usleep(GET_DATA_INTERVAL);
        }

	accelerometer_shutdown(accel);

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
	return acy;
}
double getacz()
{
	return acz;
}
