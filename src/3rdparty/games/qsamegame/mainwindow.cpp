/*
 *   KSame
 *   Copyright (C) 1997,1998  Marcus Kreutzberger <kreutzbe@informatik.mu-luebeck.de>
 *   Copyright (C) 2006 Henrique Pinto <henrique.pinto@kdemail.net>
 *   Copyright (C) 2007 Simon Hürlimann <simon.huerlimann@huerlisi.ch>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include "mainwindow.h"
#include "view.h"
#include <time.h>

#include <QAction>
#include <QMenu>
#ifdef QTOPIA_PHONE
#include <QtopiaApplication>
#include <QSoftMenuBar>
#else
#include <QApplication>
#include <QMenuBar>
#include <QInputDialog>
#endif
#include <QGraphicsScene>
#include <QTimer>
#include <QStatusBar>
#include <QSettings>
#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLCDNumber>
#include <QToolBar>
#include <QKeyEvent>
#include <QMessageBox>

static int default_colors=3;
static int default_cols=12;
static int default_rows=11;


QSame::MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags wf )
	: QMainWindow( parent, wf )
{
       
       m_toolBar = new QToolBar(this);
       m_toolBar->setFloatable(false);
       m_toolBar->setMovable(false);

       m_markedAction = m_toolBar->addWidget(m_markedLCD = new QLCDNumber( m_toolBar ) ); 
       m_markedLCD->setSegmentStyle(QLCDNumber::Flat);
       m_markedLCD->setNumDigits(3);
       m_toolBar->addWidget(m_scoreLCD = new QLCDNumber( m_toolBar ) ); 
       m_scoreLCD->setSegmentStyle(QLCDNumber::Flat);
       m_scoreLCD->setNumDigits(6);

       addToolBar(m_toolBar);

	m_board = new QSame::Board( this );
	connect( m_board, SIGNAL( scoreChanged( quint32 ) ),           this, SLOT( setScore( quint32 ) ) );
	connect( m_board, SIGNAL( newCountOfMarkedStones( int ) ),     this, SLOT( setMarked( int ) ) );
	connect( m_board, SIGNAL( gameOver() ),                        this, SLOT( gameover() ) );
	connect( m_board, SIGNAL( newGameStarted( quint32, quint8 ) ), this, SLOT( onNewGameStarted( quint32, quint8 ) ) );

	QSame::View* view = new QSame::View( m_board );
	setCentralWidget( view );

	setupActions();

	m_randomBoardAction->setChecked( true );

	QTimer::singleShot( 0, this, SLOT( newGame() ) );
	QSettings settings("QSameGame", "QSameGame");
        settings.beginGroup("General");

        if ( settings.value("showRemaining", false).toBool() ) {
		m_showNumberRemainingAction->setChecked(true);
		showNumberRemainingToggled();
	}
        settings.endGroup();
}

void QSame::MainWindow::setupActions()
{

#ifdef QTOPIA_PHONE
        QMenu *m = QSoftMenuBar::menuFor(this);
#else
        QMenu *m = menuBar()->addMenu(tr("Options"));
#endif
        QMenu *gamem = m->addMenu(tr("Game"));
        QMenu *editm = m->addMenu(tr("Edit"));
        QMenu *settingsm = m->addMenu(tr("Settings"));
        QAction *m_newAction = gamem->addAction(tr("New Game"));
        connect(m_newAction, SIGNAL(triggered(bool)), this, SLOT(newGame()));
        m_restartAction = gamem->addAction(tr("Restart Game"));
        connect(m_restartAction, SIGNAL(triggered(bool)), this, SLOT(restartGame()));
        m_undoAction = editm->addAction(tr("Undo"));
        connect(m_undoAction, SIGNAL(triggered(bool)), this, SLOT(undo()));
        m_randomBoardAction = settingsm->addAction(tr("&Random Board"));
        m_randomBoardAction->setCheckable(true);
	m_showNumberRemainingAction = settingsm->addAction(tr("&Show Number Remaining"));
        m_showNumberRemainingAction->setCheckable(true);
	connect(m_showNumberRemainingAction, SIGNAL(triggered(bool) ), this, SLOT(showNumberRemainingToggled()));

#ifndef QTOPIA_PHONE
	resize( QSize( 576, 384 ) );
#endif
	
}
/*
void QSame::MainWindow::readProperties( const KConfigGroup &conf )
{
	Q_UNUSED(conf)
	// TODO: Implement this
	// stone->readProperties(conf);
}

void QSame::MainWindow::saveProperties( KConfigGroup &conf )
{
	// TODO: Implement this
	//stone->saveProperties(conf);
	conf.sync();
}
*/

void QSame::MainWindow::showNumberRemainingToggled()
{
  for (int i=0; i<default_colors; i++) {
    if (!m_showNumberRemainingAction->isChecked()) {
      delete m_colorsLCD[i];
      m_colorsLCD[i]=0;
    } else {
      m_colorsLCD[i] = new QLCDNumber( m_toolBar );
      m_colorsLCD[i]->setSegmentStyle(QLCDNumber::Flat);
      m_colorsLCD[i]->setNumDigits(2);
      m_toolBar->insertWidget(m_markedAction,m_colorsLCD[i]);
    }
  }  
  setScore( m_board->score() );

  QSettings settings("QSameGame","QSameGame");
  settings.beginGroup("General");
  settings.setValue("showRemaining", m_showNumberRemainingAction->isChecked());
  settings.endGroup();
}

void QSame::MainWindow::newGame(unsigned int boardNumber,int colors)
{
  m_board->newGame(boardNumber, default_cols, default_rows, colors);
  setScore(0);
#ifdef QTOPIA_PHONE
#endif
}

void QSame::MainWindow::onNewGameStarted( quint32 , quint8 )
{
	setScore( m_board->score() );
        //m_colorsLCD->setText(tr( "%1 Colors").arg(colors));
        //m_boardLCD->setText(tr( "Board").arg(boardNumber));
}

bool QSame::MainWindow::confirmAbort()
{
  return m_board->isGameOver()   ||
         ( !m_board->changed() ) ||
         ( QMessageBox::question(this, tr("New Game"), 
                         tr("Do you really want to resign?"), 
                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No ) 
                             == QMessageBox::Yes );
}

void QSame::MainWindow::newGame()
{
	if ( !confirmAbort() )
		return;

	if ( m_randomBoardAction->isChecked() )
	{
          unsigned int x = time(0);
		newGame(rand_r(&x) % 1000000, default_colors );
	}
	else
	{
#ifdef QTOPIA_PHONE
		QMessageBox::information(this, tr("not supported"), tr("Feature not supported for phone edition. Please use random option"));
#else
		// Get the board number from the user
		bool ok = false;
		int newBoard = QInputDialog::getInteger( this, tr( "Select Board" ),
		                                         tr( "Select a board number:" ),
		                                         m_board->boardNumber(), 1, 1000000, 1,
		                                         &ok, 0 );

		// Start a game if the user asked for that
		if ( ok )
			newGame( newBoard, default_colors );
#endif
	}
}

void QSame::MainWindow::restartGame()
{
	if ( confirmAbort() )
		newGame( m_board->boardNumber(), m_board->colors() );
}

void QSame::MainWindow::undo()
{
	Q_ASSERT( m_board->canUndo() );
	m_board->undo();
}

void QSame::MainWindow::showHighScoreDialog()
{
	/*KScoreDialog d( KScoreDialog::Name | KScoreDialog::Score, this );
	d.addField( KScoreDialog::Custom1, tr( "Board" ), "Board" );
	d.exec();*/
}

void QSame::MainWindow::setMarked( int markedStones )
{
  m_markedLCD->display( markedStones );
}

void QSame::MainWindow::setScore( quint32 score )
{
	if ( m_showNumberRemainingAction->isChecked() )
	{
		for( int i = 1; i <= m_board->colors(); i++)
		{
                  if (m_colorsLCD[i-1]!=0)
		    m_colorsLCD[i-1]->display( m_board->count( i ) );
		}
	}
        m_scoreLCD->display( (int)score );
	m_undoAction->setEnabled( m_board->canUndo() );
	m_restartAction->setEnabled( m_board->changed() );
}

void QSame::MainWindow::gameover()
{
	m_undoAction->setEnabled( m_board->canUndo() );
	/*KScoreDialog d( KScoreDialog::Name | KScoreDialog::Score, this );
	d.addField( KScoreDialog::Custom1, tr( "Board" ), "Board" );

	KScoreDialog::FieldInfo scoreInfo;
	scoreInfo[ KScoreDialog::Score ].setNum( m_board->score() );
	scoreInfo[ KScoreDialog::Custom1 ].setNum( m_board->boardNumber() );

	if ( d.addScore(scoreInfo) )
		d.exec();*/
}

bool QSame::MainWindow::eventFilter(QEvent *event)
{
qWarning()<<"event";
	if (event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		switch (keyEvent->key()) {
			case Qt::Key_Up :
			case Qt::Key_2:
                          m_board->markNext(1);
				return true;
			case Qt::Key_Down :
			case Qt::Key_8:
                          m_board->markNext(2);
				return true;
			case Qt::Key_Left :
			case Qt::Key_4:
                          m_board->markNext(3);
				return true;
			case Qt::Key_Right :
			case Qt::Key_6:
                          m_board->markNext(4);
				return true;
			case Qt::Key_Return :
			case Qt::Key_5:
			case Qt::Key_NumberSign:
                          m_board->removeMarked();
				return true;
	       }
	}
	return false;
}

