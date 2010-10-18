/*  vibro.cpp
 *
 *  Vibro feedback routines
 *
 *  (c) 2009 Anton Olkhovik <ant007h@gmail.com>
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

#include "vibro.h"

FILE* fvibro=NULL;
pthread_t vibro_timer;
int timer_set=0;

#define VIBRATION_TIME 33

int init_vibro()
{
    /* 2.6.34 */
    fvibro = fopen("/sys/class/leds/gta02::vibrator/brightness", "w");
    if (fvibro != NULL) return 0;

    fvibro = fopen("/sys/class/leds/neo1973:vibrator/brightness", "w");
    if (fvibro != NULL) return 0;

    fvibro = fopen("/sys/devices/platform/neo1973-vibrator.0/leds/neo1973:vibrator/brightness", "w");
    if (fvibro != NULL) return 0;

    fprintf(stderr, "Vibro: can't init.\n");
    return 1;
}

void* callback(void *data)
{
    timer_set = 1;
    usleep(VIBRATION_TIME*1000);
    if (fvibro)
    {
        fprintf(fvibro, "%d", 0);
        fflush(fvibro);
    }
    timer_set = 0;
    return NULL;
}

int set_vibro(BYTE level)
{
    if (fvibro) 
    {
        if (!timer_set)
        {
            fprintf(fvibro, "%d", level);
            fflush(fvibro);
            pthread_create( &vibro_timer, NULL, callback, NULL);
        }
    }
    return 0;
}

int close_vibro()
{
    if (fvibro)
    {
        fprintf(fvibro, "%d", 0);
        fflush(fvibro);
        if (timer_set) pthread_join(vibro_timer, NULL);
        fclose(fvibro);
    }
    return 0;
}
