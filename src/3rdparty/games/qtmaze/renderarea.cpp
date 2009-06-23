/*  renderarea.cpp
 *
 *  Game field renderer
 *
 *  (c) 2009 Anton Olkhovik <ant007h@gmail.com>
 *
 *  Based on original renderarea.cpp from Qt Basic Drawing Example
 *  Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
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


#include <QtGui>

#include "paramsloader.h"
#include "renderarea.h"
#include "types.h"

Config re_game_config;
Level* re_game_levels;
int re_game_levels_count;
int re_cur_level;

 RenderArea::RenderArea(QWidget *parent)
     : QWidget(parent)
 {
    hole_pixmap.load("pics/qtmaze/hole.png");
    fin_pixmap.load("pics/qtmaze/fin.png");

    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    antialiased = false;

    re_game_config = GetGameConfig();
    re_game_levels = GetGameLevels();
    re_game_levels_count = GetGameLevelsCount();
    re_cur_level = GetUserSettings().level - 1;
 }

 void RenderArea::setLevel(int lvl_no)
 {
     re_cur_level = lvl_no;
 }

 QSize RenderArea::minimumSizeHint() const
 {
     return QSize(100, 100);
 }

 QSize RenderArea::sizeHint() const
 {
     return QSize(400, 200);
 }

 void RenderArea::setAntialiased(bool antialiased)
 {
     this->antialiased = antialiased;
     update();
 }

 void RenderArea::paintEvent(QPaintEvent * /* event */)
 {

    QPainter painter(this);
    //painter.setPen(pen);
    //painter.setPen(palette().dark().color());
    //painter.setBrush(brush);
    painter.setPen(Qt::NoPen);
    //painter.setBrush(QColor(250, 250, 250, 90));
    painter.setBrush(QColor(110, 110, 110, 255));
    if (antialiased)
    {
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.translate(+0.5, +0.5);
    }

    for (int i=0; i<re_game_levels[re_cur_level].boxes_count; i++)
    {
        Box *box = &re_game_levels[re_cur_level].boxes[i];
        QRect rect(
                    box->x1, box->y1,
                    box->x2 - box->x1, box->y2 - box->y1
                   );
        painter.drawRect(rect);
    }

    for (int i=0; i<re_game_levels[re_cur_level].holes_count; i++)
    {
        Point *hole = &re_game_levels[re_cur_level].holes[i];
        painter.drawPixmap( hole->x - re_game_config.hole_r,
                            hole->y - re_game_config.hole_r,
                            hole_pixmap);
    }

    Point *fin = &re_game_levels[re_cur_level].fins[0];
    painter.drawPixmap( fin->x - re_game_config.hole_r,
                        fin->y - re_game_config.hole_r,
                        fin_pixmap);

 }
