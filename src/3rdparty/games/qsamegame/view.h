/*******************************************************************
 *
 * Copyright (C) 2006 Henrique Pinto <henrique.pinto@kdemail.net>
 * 
 * This file is part of the KDE project
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with his program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 ********************************************************************/
#ifndef VIEW_H
#define VIEW_H

#include "renderer.h"
#include "board.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSize>

namespace QSame
{
	/**
	 * A view for QSame::Board scenes.
	 */
	class View: public QGraphicsView
	{
		public:
			/* Constructor. Creates a view for the board */
			explicit View( QSame::Board *board, QWidget *parent = 0 );
			/* Destructor */
			~View();

		protected:
			/* Informs the board about size changes */
			virtual void resizeEvent( QResizeEvent *event );

			virtual QSize minimumSizeHint() const;
		private:
			/* The board associated with this view */
			QSame::Board *m_board;
	};
} // namespace QSame

#endif // VIEW_H
