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

#ifndef QSAME_MAINWINDOW_H
#define QSAME_MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QEvent;
class QLCDNumber;

namespace QSame
{
	class Board;

	class MainWindow: public QMainWindow
	{
		Q_OBJECT

		public:
		  MainWindow( QWidget *parent = 0 , Qt::WindowFlags wf = 0);

		private slots:
			void newGame();
			void onNewGameStarted( quint32 boardNumber, quint8 colors );
			void restartGame();
			void showHighScoreDialog();
			void undo();

			void gameover();
			void setScore(quint32 score);
			void setMarked(int m);

			void showNumberRemainingToggled();

		protected:
			void newGame(unsigned int board, int colors);
			void setupActions();
			/*virtual void saveProperties(KConfigGroup &conf);
			virtual void readProperties(const KConfigGroup &conf);*/

			bool confirmAbort();
                        bool eventFilter(QEvent *event);

		private:
			QSame::Board *m_board;
			QToolBar   *m_toolBar;
                        QLCDNumber   *m_colorsLCD[4];
                        QLCDNumber   *m_scoreLCD;
                        QLCDNumber   *m_markedLCD;
                        QAction *m_markedAction;

			QAction *m_randomBoardAction;
			QAction *m_showNumberRemainingAction;
			QAction *m_restartAction;
			QAction *m_undoAction;
	};

} // namespace QSame

#endif // QSAME_MAINWINDOW_H
