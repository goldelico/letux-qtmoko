/*  paramsloader.h
 *
 *  Config and levelpack loader
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

#ifndef PARAMSLOADER_H
#define PARAMSLOADER_H

#include "types.h"

void parse_command_line(int argc, char *argv[]);
int load_params();
Config GetGameConfig();
Level* GetGameLevels();
User GetUserSettings();
int GetGameLevelsCount();
int GetVibroEnabled();
Prompt GetArguments();
char* GetExecInit();
char* GetExecFinal();

void SaveLevel(int n);

#endif

