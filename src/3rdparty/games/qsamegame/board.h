/*******************************************************************
 *
 * Copyright (C) 1997,1998  Marcus Kreutzberger <kreutzbe@informatik.mu-luebeck.de>
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
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 ********************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include "renderer.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QSize>
#include <QVector>
#include <QGraphicsPixmapItem>
#include <QStack>
#include <QPair>
#include <QKeyEvent>


namespace QSame
{
	class Board;

	class Stone: public QGraphicsPixmapItem
	{
		public:
			Stone( QSame::Board *board, int x, int y, QGraphicsItem *parent = 0 );

		protected:
			virtual void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
			virtual void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
			virtual void mousePressEvent( QGraphicsSceneMouseEvent * event );
			virtual void keyPressEvent( QKeyEvent *event);

		private:
			int m_x, m_y;
			QSame::Board *m_board;
	};

	typedef QPair<int, int> Coordinate;

	class GameState
	{
		public:
			GameState(): m_valid( false ) {}
			GameState( QSame::Board *board );

			QVector<quint8> m_boardData;
			quint8  m_width;
			quint8  m_height;
			quint8  m_colorCount;
			quint32 m_boardNumber;
			quint32 m_score;
			bool    m_valid;
			bool    m_changed;
	};

	class Board: public QGraphicsScene
	{
		friend class QSame::Stone;
		friend class QSame::GameState;

		Q_OBJECT

		public:
			explicit Board( QObject *parent = 0 );

			void newGame( quint32 boardNumber = 0, quint8 width = 15, quint8 height = 10, quint8 colorCount = 3 );
			void resize( const QSize& size );

			quint32 width()  const { return m_width;  }
			quint32 height() const { return m_height; }
			quint32 score()  const { return m_score;  }
			quint32 boardNumber() const { return m_boardNumber; }
			quint8  colors() const { return m_colorCount; }
			quint8  count( quint8 color ) const { return m_boardData.count( color ); }

			int markedStones() const { return m_markedStones.count(); }

			void markNext(int dir);
                        void displayCursor();
			void removeMarked();

			bool changed() const { return m_changed; }
			bool canUndo() const { return !m_undoList.empty(); }
			bool undo();

			bool isGameOver() const;
			bool won() const;

		signals:
			void newGameStarted( quint32 boardNumber, quint8 colors );
			void newCountOfMarkedStones( int markedStones );
			void stonesRemoved( int numberOfRemovedStones );
			void gameOver();
			void scoreChanged( quint32 newScore );

		protected:
			virtual void drawBackground( QPainter *painter, const QRectF& rect );

			void mark( int x, int y );
                        bool isMarked( int x, int y);
			void unmark();

		private:
			void initializeBoardData();
			void createItems();
			void generateGameOverPixmap( bool won );
			void markHelper( int x, int y, quint8 color );
			int  map( int x, int y ) const;
			bool validPosition( int x, int y ) const;

			QSame::Renderer m_renderer;

			quint8  m_width;
			quint8  m_height;
			quint8  m_colorCount;
			quint32 m_boardNumber;
			quint32 m_score;
			bool    m_changed;

			QVector<quint8>          m_boardData;
			QVector<QSame::Stone*>   m_stones;
			QSize                    m_elementsSize;
			QList<QSame::Coordinate> m_markedStones;
			QStack<QSame::GameState> m_undoList;
                        QSame::Coordinate	 m_cursor;

			QGraphicsPixmapItem *m_gameOverOverlay;
	};

} // namespace QSame

#endif // BOARD_H
