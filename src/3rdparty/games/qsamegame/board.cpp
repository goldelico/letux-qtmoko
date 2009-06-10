/*******************************************************************
 *
 * Copyright (C) 1997,1998  Marcus Kreutzberger <kreutzbe@informatik.mu-luebeck.de>
 * Copyright (C) 2006 Henrique Pinto <henrique.pinto@kdemail.net>
 * Copyright (C) 2006 Stephan Kulow <coolo@kde.org>
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
#include "view.h"

#include <QDebug>
#ifdef QTOPIA_PHONE
#include <Qtopia>
#endif

QSame::Stone::Stone( QSame::Board *board, int x, int y, QGraphicsItem *parent )
	: QGraphicsPixmapItem( parent, board ), m_x( x ), m_y( y ), m_board( board )
{
	setAcceptsHoverEvents( true );
	setAcceptedMouseButtons( Qt::LeftButton );
	if (!Qtopia::mousePreferred())
        	setFlag(QGraphicsItem::ItemIsFocusable, true);
}

void QSame::Stone::hoverEnterEvent( QGraphicsSceneHoverEvent * )
{
#ifndef QTOPIA_PHONE
  m_board->mark( m_x, m_y );
#endif
}

void QSame::Stone::hoverLeaveEvent( QGraphicsSceneHoverEvent * )
{
#ifndef QTOPIA_PHONE
  m_board->unmark();
#endif
}

void QSame::Stone::mousePressEvent( QGraphicsSceneMouseEvent * )
{
#ifdef QTOPIA_PHONE
  if ( m_board->isMarked( m_x, m_y )) {
    m_board->removeMarked();
  } else {
    m_board->unmark();
    m_board->mark( m_x, m_y );
  }
#else
  m_board->removeMarked();
#endif
}

void QSame::Stone::keyPressEvent( QKeyEvent *keyEvent)
{
#ifdef QTOPIA_PHONE
  switch (keyEvent->key()) {
    case Qt::Key_Up :
    case Qt::Key_2:
        m_board->markNext(1);
        break;
    case Qt::Key_Down :
    case Qt::Key_8:
        m_board->markNext(2);
        break;
    case Qt::Key_Left :
    case Qt::Key_4:
        m_board->markNext(3);
        break;
    case Qt::Key_Right :
    case Qt::Key_6:
        m_board->markNext(4);
        break;
    case Qt::Key_Return :
    case Qt::Key_5:
    case Qt::Key_NumberSign:
    case Qt::Key_Select:
        m_board->removeMarked();
        break;
    case Qt::Key_Back :
        qApp->exit();
        break;
  }
#endif
}

QSame::GameState::GameState( QSame::Board *board )
{
	m_boardData   = board->m_boardData;
	m_width       = board->m_width;
	m_height      = board->m_height;
	m_colorCount  = board->m_colorCount;
	m_boardNumber = board->m_boardNumber;
	m_score       = board->m_score;
	m_changed     = board->m_changed;
	m_valid       = true;
}

QSame::Board::Board( QObject *parent )
	: QGraphicsScene( parent ),
#ifdef QTOPIA_PHONE
	/* FIXME: need to find a way to load the pic file or at least the path*/
	  m_renderer( "/opt/Qtopia/pics/qsamegame/default_theme.svg" , QSize(), QSize( 64, 128 ), this ),
#else
	  m_renderer( "./pics/default_theme.svg" , QSize(), QSize( 64, 128 ), this ),
#endif
	  m_width( 0 ), m_height( 0 ), m_colorCount( 0 ), m_boardNumber( 0 ),
	  m_score( 0 ), m_changed( false ), m_boardData( 0 )
{
	m_elementsSize = QSize( 64, 64 );
	m_gameOverOverlay = new QGraphicsPixmapItem;
	addItem( m_gameOverOverlay );
	m_gameOverOverlay->setZValue( 20000 );
	m_gameOverOverlay->hide();
}

void QSame::Board::drawBackground( QPainter *painter, const QRectF& )
{
	painter->drawPixmap( 0, 0, m_renderer.renderBackground() );
}

void QSame::Board::newGame( quint32 boardNumber, quint8 width, quint8 height, quint8 colorCount )
{
	qDebug();
	m_boardData.resize( width * height );
	m_boardNumber = boardNumber;
	m_width       = width;
	m_height      = height;
	m_colorCount  = colorCount;
	m_score       = 0;
	m_changed     = false;
	m_undoList.clear();
	m_markedStones.clear();
	initializeBoardData();
	createItems();
	m_gameOverOverlay->hide();
        m_cursor = qMakePair( 0, 0 );
        displayCursor();
	emit newGameStarted( m_boardNumber, m_colorCount );
}

void QSame::Board::resize( const QSize& size )
{
	setSceneRect( 0, 0, size.width(), size.height() );
	m_renderer.setBackgroundSize( size );

	if ( m_changed && isGameOver() )
	{
		generateGameOverPixmap( won() );
	}

	m_renderer.setElementSize( m_elementsSize );
	createItems();
	displayCursor();
}

void QSame::Board::initializeBoardData()
{
	/* Pass the board number as seed */
        unsigned int x = m_boardNumber;

	/* Randomly choose a color for each stone in the board */

	for ( int i = 0; i < m_width * m_height; ++i )
	{
		m_boardData[ i ] = 1 + rand_r(&x) % m_colorCount;
	}
}

bool QSame::Board::validPosition( int x, int y ) const
{
	return ( ( x >= 0 ) && ( x < m_width  )
	         &&
	         ( y >= 0 ) && ( y < m_height ) );
}

int QSame::Board::map( int x, int y ) const
{
	Q_ASSERT( validPosition( x, y ) );
	return x*m_height + y;
}

void QSame::Board::createItems()
{
	if ( !m_width || !m_height )
		return;

	if ( sceneRect().height()/m_height < sceneRect().width()/m_width )
	{
		m_elementsSize = QSize( static_cast<int>( sceneRect().height()/m_height ),
		                        static_cast<int>( sceneRect().height()/m_height ) );
	}
	else
	{
		m_elementsSize = QSize( static_cast<int>( sceneRect().width()/m_width ),
		                        static_cast<int>( sceneRect().width()/m_width ) );
	}
	m_renderer.setElementSize( m_elementsSize );


	/* Remove current items, if any */
	foreach( QSame::Stone *item, m_stones )
	{
		removeItem( item );
		delete item;
	}

	m_stones.resize( m_width * m_height );

	/* Create an item for each stone in the board */
	for ( int i = 0; i < m_width; ++i )
	{
		for ( int j = 0; j < m_height; ++j )
		{
			int index = map( i, j );
			QSame::Stone *item = new QSame::Stone( this, i, j );
			if ( m_boardData[ index ] )
				item->setPixmap( m_renderer.renderElement( QString( "stone%1" ).arg( m_boardData[ index ] ) ) );
			item->setPos( i*m_elementsSize.width(), (( m_height - 1 - j )*m_elementsSize.height()));
			m_stones[ index ] = item;
		}
	}
}

void QSame::Board::mark( int x, int y )
{
	int index = map( x, y );
	quint8 color = m_boardData[ index ];

	markHelper( x, y, color );

	if ( m_markedStones.count() < 2 )
		m_markedStones.clear();

	foreach( const QSame::Coordinate &s, m_markedStones )
	{
		m_stones[ map( s.first, s.second ) ]->setPixmap( m_renderer.renderHighlightedElement( QString( "stone%1" ).arg( color ) ) );
	}
	emit newCountOfMarkedStones( m_markedStones.count() );
}

bool QSame::Board::isMarked( int x, int y )
{
  return m_markedStones.contains( qMakePair( x, y ) );
}

void QSame::Board::markHelper( int x, int y, quint8 color )
{
	if ( !validPosition( x, y ) )
		return;

	int index = map( x, y );

	if ( ( m_boardData[ index ] == color ) && !m_markedStones.contains( qMakePair( x, y ) ) )
	{
		// Add this stone to the list of marked stones
		m_markedStones << qMakePair( x, y );
		// Check if the left neighbor should be marked
		markHelper( x - 1, y, color );
		// Check if the right neighbor should be marked
		markHelper( x + 1, y, color );
		// Check if the top neighbor should be marked
		markHelper( x, y + 1, color );
		// Check if the down neighbor should be marked
		markHelper( x, y - 1, color );
	}
}

void QSame::Board::unmark()
{
	foreach( const QSame::Coordinate &s, m_markedStones )
	{
		int index = map( s.first, s.second );
		QString elementId = QString( "stone%1" ).arg( m_boardData[ index ] == 0? "_removed" : QString::number( m_boardData[ index ] ) );
		m_stones[ index ]->setPixmap( m_renderer.renderElement( elementId ) );
	}
        int index = map( m_cursor.first, m_cursor.second ); 
	QString elementId = QString( "stone%1" ).arg( m_boardData[ index ] == 0? "_removed" : QString::number( m_boardData[ index ] ) );
	m_stones[ index ]->setPixmap( m_renderer.renderElement( elementId ) );

	m_markedStones.clear();
	emit newCountOfMarkedStones( m_markedStones.count() );
}

void QSame::Board::removeMarked()
{
	if ( m_markedStones.count() < 2 )
		return;

	// Add the current state to the undo list
	m_undoList.push( QSame::GameState( this ) );

	// Increase the score
	m_score += ( m_markedStones.count() - 2 ) * ( m_markedStones.count() - 2 );

	// Remove the marked stones
	foreach( const QSame::Coordinate &s, m_markedStones )
	{
		m_boardData[ map( s.first, s.second ) ] = 0;
	}

	// Gravity
	for ( int column = 0; column < m_width; ++column )
	{
		for ( int i = 1; i < m_height; ++i )
		{
			int index = map( column, i );
			qint8 color= m_boardData[ index ];
			int j = i - 1;
			while ( ( j >= 0 ) && ( ( color > 0 ) && ( m_boardData[ map( column, j ) ] == 0 ) ) )
			{
				m_boardData[ map( column, j + 1 ) ] = m_boardData[ map( column, j ) ];
				--j;
			}
			m_boardData[ map( column, j + 1 ) ] = color;
		}
	}

	// Remove empty columns
	for ( int i = 1; i < m_width; ++i )
	{
		if ( ( m_boardData[ map( i - 1, 0 ) ] ) || ( !m_boardData[ map( i, 0 ) ] ) )
			continue;

                int j;
		QVector<quint8> columnData( m_height );
		for ( j = 0; j < m_height; ++j )
			columnData[ j ] = m_boardData[ map( i, j ) ];
		j = i - 1;
		while ( ( j >= 0 ) && ( m_boardData[ map( j, 0 ) ] == 0 ) )
		{
			for ( int k = 0; k < m_height; ++k )
				m_boardData[ map( j+1, k ) ] = m_boardData[ map( j, k ) ];
			--j;
		}
		for ( int k = 0; k < m_height; ++k )
			m_boardData[ map( j+1, k ) ] = columnData[ k ];
	}

	// If the board is empty, give a bonus
	if ( m_boardData[ map( 0, 0 ) ] == 0 )
	{
		m_score += 1000;
	}

	emit scoreChanged( m_score );
	emit stonesRemoved( m_markedStones.count() );

	m_markedStones.clear();
	emit newCountOfMarkedStones( m_markedStones.count() );
	m_changed = true;

	createItems();

	if ( isGameOver() )
	{
		m_undoList.clear();
		generateGameOverPixmap( won() );
		m_gameOverOverlay->show();
		emit gameOver();
	} else {
          displayCursor();
        }
}

bool QSame::Board::isGameOver() const
{
	for ( int column = 0; column < m_width; ++column )
	{
		for ( int row = 0; row < m_height; ++row )
		{
			quint8 color = m_boardData[ map( column, row ) ];
			if ( !color )
			{
				break;
			}
			if ( validPosition( column, row+1 ) )
			{
				if ( m_boardData[ map( column, row+1 ) ] == color )
				{
					return false;
				}
			}
			if ( validPosition( column+1, row ) )
			{
				if ( m_boardData[ map( column+1, row ) ] == color )
				{
					return false;
				}
			}
		}
	}
	return true;
}

bool QSame::Board::won() const
{
	Q_ASSERT( isGameOver() );
	return ( m_boardData[ map( 0, 0 ) ] == 0 );
}

bool QSame::Board::undo()
{
	if ( !canUndo() )
		return false;

	QSame::GameState lastState = m_undoList.pop();
	Q_ASSERT( lastState.m_valid );

	m_width       = lastState.m_width;
	m_height      = lastState.m_height;
	m_colorCount  = lastState.m_colorCount;
	m_boardNumber = lastState.m_boardNumber;
	m_score       = lastState.m_score;
	m_changed     = lastState.m_changed;
	m_boardData   = lastState.m_boardData;

	createItems();
	m_gameOverOverlay->hide();
	m_markedStones.clear();
	emit newCountOfMarkedStones( m_markedStones.count() );
	emit scoreChanged( lastState.m_score );
	return true;
}

void QSame::Board::generateGameOverPixmap( bool won )
{
	qDebug() ;

	int itemWidth  = qRound( 0.8*sceneRect().width()  );
	int itemHeight = qRound( 0.6*sceneRect().height() );

	QPixmap px( itemWidth, itemHeight );
	qDebug() << "Pixmap Size:" << px.size();
	px.fill( QColor( 0, 0, 0, 0 ) );

	QPainter p( &px );
	p.setPen( QColor( 0, 0, 0, 0 ) );
	p.setBrush( QBrush( QColor( 188, 202, 222, 155 ) ) );
	p.setRenderHint( QPainter::Antialiasing );
	p.drawRoundRect( 0, 0, itemWidth, itemHeight, 25 );

	QString text;
	if ( won )
		text = tr( "You won.\nYou even removed the last stone, great job!\n Your score was %1.").arg(m_score);
	else
		text = tr( "Game Over.\nThere are no more\n removable stones.\n Your score was %1.").arg(m_score );

	QFont font;
	font.setPointSize( 28 );
	p.setFont( font );
	int textWidth = p.boundingRect( p.viewport(), Qt::AlignCenter|Qt::AlignVCenter, text ).width();
	int fontSize = 28;
	while ( ( textWidth > itemWidth * 0.95 ) && ( fontSize > 12 ) )
	{
		fontSize--;
		font.setPointSize( fontSize );
		p.setFont( font );
		textWidth = p.boundingRect( p.viewport(), Qt::AlignCenter|Qt::AlignVCenter, text ).width();
	}

	p.setPen( QColor( 0, 0, 0, 255 ) );
	p.drawText( p.viewport(), Qt::AlignCenter|Qt::AlignVCenter, text );
	p.end();

	m_gameOverOverlay->setPixmap( px );
	m_gameOverOverlay->setPos( ( sceneRect().width() - itemWidth )/2, ( sceneRect().height() - itemHeight )/2 );

}

void QSame::Board::markNext(int dir) {
  unmark();
  switch (dir) {
    case 1: /* UP */
      if (validPosition(m_cursor.first,m_cursor.second+1))
        {
          m_cursor = qMakePair(m_cursor.first, m_cursor.second+1);
        }
      break;
    case 2: /* DOWN */
      if (validPosition(m_cursor.first,m_cursor.second-1))
        {
          m_cursor = qMakePair(m_cursor.first, m_cursor.second-1);
        }
      break;
    case 3: /* LEFT */
      if (validPosition(m_cursor.first-1,m_cursor.second))
        {
          m_cursor = qMakePair(m_cursor.first-1, m_cursor.second);
        }
      break;
    case 4: /* RIGHT */
      if (validPosition(m_cursor.first+1,m_cursor.second))
        {
          m_cursor = qMakePair(m_cursor.first+1, m_cursor.second);
        }
      break;
  }  
  displayCursor();
}

void QSame::Board::displayCursor() 
{
#ifdef QTOPIA_PHONE
  if (Qtopia::mousePreferred()) return;

  int index = map( m_cursor.first, m_cursor.second );
  qint8 color = m_boardData[ index ];

  for (int i=m_cursor.first; i>=0; i--)
    for (int j=m_cursor.second; j>=0; j--) {
       index = map(i,j);
       color = m_boardData[ index ];
       if (color) {
         m_cursor = qMakePair(i,j);
         mark(i,j);
         m_stones[ map( i, j ) ]->setPixmap( m_renderer.renderCursorElement( QString( "stone%1" ).arg( color ) ) );
         m_stones[ map( i, j ) ]->setFocus();
         return;
       }
    }
#endif
}

