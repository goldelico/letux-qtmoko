//  qnetwalk/mainwindow.cpp
//  Copyright (C) 2004, Andi Peredri <andi@ukr.net>
//  Ported to Qt4 by xep

#include "mainwindow.h"
#include <QDebug>
#include <QAction>
#include <QApplication>
#include <QEventLoop>
#include <QLabel>
#include <QLayout>
#include <QLCDNumber>
#include <QLineEdit>
#ifdef QTOPIA_PHONE
#include <QSoftMenuBar>
#else
#include <QMenuBar>
#endif
#include <QMessageBox>
#include <QProcess>
#include <QPushButton>
#include <QSettings>
#include <QSound>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>
#include <QDir>
#include <QFile>
#include <QGridLayout>
#include <QPixmap>
#include <QFrame>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cell.h"

static QMap<Cell::Dirs, Cell::Dirs> contrdirs;

MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags wf) 
  : QMainWindow(parent, wf)
{
  //setMinimumSize(geometry().size());
  //setMaximumSize(geometry().size());

  contrdirs[Cell::U] = Cell::D;
  contrdirs[Cell::R] = Cell::L;
  contrdirs[Cell::D] = Cell::U;
  contrdirs[Cell::L] = Cell::R;

#ifndef QTOPIA_PHONE
  QString appdir = qApp->applicationDirPath();
  soundpath = appdir + "/sounds/";
  if(!QFile::exists(soundpath))
    soundpath = appdir + "/../share/qnetwalk/sounds/";
  winsound   = new QSound(soundpath + "win.wav");
  turnsound   = new QSound(soundpath + "turn.wav");
  clicksound   = new QSound(soundpath + "click.wav");
  startsound   = new QSound(soundpath + "start.wav");
  connectsound = new QSound(soundpath + "connect.wav");
#else
  QString appdir = qApp->applicationDirPath();
  soundpath = ":sound/qnetwalk/";

  winsound   = new QSound(soundpath + "win");
  turnsound   = new QSound(soundpath + "turn");
  clicksound   = new QSound(soundpath + "click");
  startsound   = new QSound(soundpath + "start");
  connectsound = new QSound(soundpath + "connect");
#endif

  QSettings settings("QNetWalk", "QNetWalk");
  settings.beginGroup("QNetWalk");
  username = settings.value("Username", getenv("USER")).toString();
  bool issound = settings.value("Sound", true).toBool();

  highscores = settings.value("Highscores").toStringList();
  if(highscores.count() != NumHighscores * 8)
  {
    highscores.clear();
    for(int i = 1; i < 5; i++)
    {
      for(int scores = 20 * i; scores < 30 * i; scores += i)
      {
        highscores.append("...");
        highscores.append(QString::number(scores));
      }
    }
  }

  skill = settings.value("Skill", Novice).toInt();
  if(skill != Novice && skill != Normal && skill != Expert)
    skill = Master;
  settings.endGroup();

  for(int i = 1; i < qApp->argc(); i++)
  {
    QString argument = qApp->argv()[i];
    if(argument == "-novice")
      skill = Novice;
    else if(argument == "-amateur")
      skill = Normal;
    else if(argument == "-expert")
      skill = Expert;
    else if(argument == "-master")
      skill = Master;
    else if(argument == "-nosound")
      issound = false;
  }

  setWindowTitle(tr("QNetWalk"));
  setWindowIcon(QPixmap(":/qnetwalk.png"));

#ifdef QTOPIA_PHONE
  QMenu *m = QSoftMenuBar::menuFor(this);
#else
  QMenu *m = menuBar()->addMenu(tr("Options"));
#endif  
  soundaction = new QAction(tr("&Sound"), this);
  soundaction->setCheckable(true);
  soundaction->setChecked(issound);

  gamemenu = m->addMenu(tr("&Game"));
  gamemenu->addAction(QPixmap(":/newgame.png"), tr("&New"), this, SLOT(newGame()));
  gamemenu->addAction(QPixmap(":/highscores.png"), tr("&Highscores"), this, SLOT(showHighscores()));
  gamemenu->addAction(soundaction);
#ifndef QTOPIA_PHONE
  gamemenu->addSeparator();
  gamemenu->addAction(QPixmap(":/quit.png"), tr("&Quit"), qApp, SLOT(closeAllWindows()));
#endif

  skillmenu = m->addMenu(tr("&Skill"));
  skillActionGroup = new QActionGroup(this);
  QAction* action;
  action = skillActionGroup->addAction(tr("&Novice"));
  action->setCheckable(true);
  if(skill == Novice)
    action->setChecked(true);
  connect(action, SIGNAL(triggered()), this, SLOT(setSkillNovice()));
  action = skillActionGroup->addAction(tr("&Amateur"));
  action->setCheckable(true);
  if(skill == Normal)
    action->setChecked(true);
  connect(action, SIGNAL(triggered()), this, SLOT(setSkillNormal()));
  action = skillActionGroup->addAction(tr("&Expert"));
  action->setCheckable(true);
  if(skill == Expert)
    action->setChecked(true);
  connect(action, SIGNAL(triggered()), this, SLOT(setSkillExpert()));
  action = skillActionGroup->addAction(tr("&Master"));
  action->setCheckable(true);
  if(skill == Master)
    action->setChecked(true);
  connect(action, SIGNAL(triggered()), this, SLOT(setSkillMaster()));
  skillmenu->addActions(skillActionGroup->actions());

  QMenu* helpmenu = m->addMenu(tr("&Help"));
  helpmenu->addAction(tr("&Rules of Play"), this, SLOT(help()));
  helpmenu->addAction(QPixmap(":/homepage.png"), tr("&Homepage"), this, SLOT(openHomepage()));
  helpmenu->addSeparator();
  helpmenu->addAction(QPixmap(":/qnetwalk.png"), tr("&About QNetWalk"), this, SLOT(about()));
  helpmenu->addAction(tr("About &Qt"), qApp, SLOT(aboutQt()));

  QToolBar* toolbar = new QToolBar(this);
  toolbar->setFloatable(false);
  toolbar->setMovable(false);
  toolbar->addAction(QPixmap(":/newgame.png"), tr("New Game"), this, SLOT(newGame()));
  toolbar->addAction(QPixmap(":/highscores.png"), tr("Show Highscores"), this, SLOT(showHighscores()));
  lcd = new QLCDNumber(toolbar);
  lcd->setFrameStyle(QFrame::Plain);
  toolbar->addWidget(lcd);

  addToolBar(toolbar);

  Cell::initPixmaps();
  srand(time(0));
  setSkill(skill);
  installEventFilter(this);
  resize(240,320);
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    switch (keyEvent->key()) {
      case Qt::Key_Up : 
      case Qt::Key_2:
           board[selected]->setSelected(false);
           if (selected - BoardSize < 0) {
             selected += BoardSize*(BoardSize-1);
           } else
             selected -= BoardSize;
           board[selected]->setSelected(true);
           return true;
      case Qt::Key_Down : 
      case Qt::Key_8:
           board[selected]->setSelected(false);
           if (selected + BoardSize >= BoardSize*BoardSize) {
             selected -= BoardSize*(BoardSize-1);
           } else 
             selected += BoardSize;
           board[selected]->setSelected(true);
           return true;
      case Qt::Key_Left : 
      case Qt::Key_4:
           board[selected]->setSelected(false);
           if (selected  % BoardSize == 0) {
             selected += BoardSize - 1;
           } else
             selected -= 1;
           board[selected]->setSelected(true);
           return true;
      case Qt::Key_Right : 
      case Qt::Key_6:
           board[selected]->setSelected(false);
           if ((selected + 1) % BoardSize == 0) {
             selected -= BoardSize - 1;
           } else
             selected += 1;
           board[selected]->setSelected(true);
           return true;
      case Qt::Key_Return :
      case Qt::Key_5:
      case Qt::Key_NumberSign:
           emit rClicked(selected);
           return true;
      case Qt::Key_Asterisk:
           emit lClicked(selected);
    }
  }
  return false;
}

void MainWindow::initCells()
{

  if (centralWidget() != 0) {
    delete centralWidget();
  }
  QWidget *grid = new QWidget();
  QPalette pal;
  pal.setBrush(QPalette::Active, QPalette::Window,QBrush(QColor("#b2b5c7")));
  grid->setPalette(pal);
  grid->setAutoFillBackground(true);
  grid->setBackgroundRole(QPalette::Window);

  QGridLayout* layout = new QGridLayout;
  layout->setHorizontalSpacing(0);
  layout->setVerticalSpacing(0);
  layout->setSizeConstraint(QLayout::SetNoConstraint);
  layout->setContentsMargins(1,1,1,1);
  for(int i = 0; i < BoardSize; i++)
  {
    for(int j = 0; j < BoardSize; j++)
    {
      int ii = i * BoardSize + j;
      board[ii] = new Cell(grid, ii);
      connect(board[ii], SIGNAL(lClicked(int)), SLOT(lClicked(int)));
      connect(board[ii], SIGNAL(rClicked(int)), SLOT(rClicked(int)));
      layout->addWidget(board[ii], i, j);
    }
  }
  
  grid->setLayout(layout); 
  setCentralWidget(grid);
  QApplication::processEvents();
  doResize();
}

void MainWindow::newGame()
{
  lcd->display(0);
  if(soundaction->isChecked())
    startsound->play();

  for(int i = 0; i < BoardSize * BoardSize; i++)
  {
    board[i]->setDirs(Cell::None);
    board[i]->setConnected(false);
    board[i]->setRoot(false);
    board[i]->setSelected(false);
  }

  const int start = 0;
  const int rootrow = rand() % BoardSize;
  const int rootcol = rand() % BoardSize;

  root = board[(start + rootrow) * BoardSize + rootcol];
  selected = (start + rootrow) * BoardSize + rootcol;
  root->setConnected(true);
  root->setRoot(true);
  root->setSelected(true);

  while(true)
  {
    for(int row = start; row < start + BoardSize; row++)
      for(int col = start; col < start + BoardSize; col++)
        board[row * BoardSize + col]->setDirs(Cell::Free);

    CellList list;
    list.append(root);
    if(rand() % 2) addRandomDir(list);

    while(!list.isEmpty())
    {
      if(rand() % 2)
      {
        addRandomDir(list);
        if(rand() % 2)
          addRandomDir(list);
        list.removeFirst();
      }
      else
      {
        list.append(list.first());
        list.removeFirst();
      }
    }

    int cells = 0;
    for(int i = 0; i < BoardSize * BoardSize; i++)
    {
      Cell::Dirs d = board[i]->dirs();
      if(d != Cell::Free && d != Cell::None)
        cells++;
    }
    if(cells >= MinimumNumCells)
      break;
  }

  for(int i = 0; i < BoardSize * BoardSize; i++)
    board[i]->rotate((rand() % 4) * 90);
  updateConnections();
}

bool MainWindow::updateConnections()
{
  bool newconnection[BoardSize * BoardSize];
  for(int i = 0; i < BoardSize * BoardSize; i++)
    newconnection[i] = false;

  CellList list;
  if(!root->isRotated())
  {
    newconnection[root->index()] = true;
    list.append(root);
  }
  while(!list.isEmpty())
  {
    Cell* cell = list.first();
    Cell* ucell = uCell(cell);
    Cell* rcell = rCell(cell);
    Cell* dcell = dCell(cell);
    Cell* lcell = lCell(cell);

    if((cell->dirs() & Cell::U) && ucell && (ucell->dirs() & Cell::D) && !newconnection[ucell->index()] && !ucell->isRotated())
    {
      newconnection[ucell->index()] = true;
      list.append(ucell);
    }
    if((cell->dirs() & Cell::R) && rcell && (rcell->dirs() & Cell::L) && !newconnection[rcell->index()] && !rcell->isRotated())
    {
      newconnection[rcell->index()] = true;
      list.append(rcell);
    }
    if((cell->dirs() & Cell::D) && dcell && (dcell->dirs() & Cell::U) && !newconnection[dcell->index()] && !dcell->isRotated())
    {
      newconnection[dcell->index()] = true;
      list.append(dcell);
    }
    if((cell->dirs() & Cell::L) && lcell && (lcell->dirs() & Cell::R) && !newconnection[lcell->index()] && !lcell->isRotated())
    {
      newconnection[lcell->index()] = true;
      list.append(lcell);
    }
    list.removeFirst();
  }

  bool isnewconnection = false;
  for(int i = 0; i < BoardSize * BoardSize; i++)
  {
    if(!board[i]->isConnected() && newconnection[i])
      isnewconnection = true;
    board[i]->setConnected(newconnection[i]);
  }

  return isnewconnection;
}

void MainWindow::addRandomDir(CellList& list)
{
  Cell* cell = list.first();
  Cell* ucell = uCell(cell);
  Cell* rcell = rCell(cell);
  Cell* dcell = dCell(cell);
  Cell* lcell = lCell(cell);

  typedef QMap<Cell::Dirs, Cell*> CellMap;
  CellMap freecells;

  if(ucell && ucell->dirs() == Cell::Free)
    freecells[Cell::U] = ucell;
  if(rcell && rcell->dirs() == Cell::Free)
    freecells[Cell::R] = rcell;
  if(dcell && dcell->dirs() == Cell::Free)
    freecells[Cell::D] = dcell;
  if(lcell && lcell->dirs() == Cell::Free)
    freecells[Cell::L] = lcell;
  if(freecells.isEmpty())
    return;

  CellMap::ConstIterator it = freecells.constBegin();
  for(int i = rand() % freecells.count(); i > 0; --i)
    ++it;

  cell->setDirs(Cell::Dirs(cell->dirs() | it.key()));
  it.value()->setDirs(contrdirs[it.key()]);
  list.append(it.value());
}

Cell* MainWindow::uCell(Cell* cell) const
{
  if(cell->index() >= BoardSize)
    return board[cell->index() - BoardSize];
  else if(wrapped)
    return board[BoardSize * (BoardSize - 1) + cell->index()];
  else
    return 0;
}

Cell* MainWindow::dCell(Cell* cell) const
{
  if(cell->index() < BoardSize * (BoardSize - 1))
    return board[cell->index() + BoardSize];
  else if(wrapped)
    return board[cell->index() - BoardSize * (BoardSize - 1)];
  else
    return 0;
}

Cell* MainWindow::lCell(Cell* cell) const
{
  if(cell->index() % BoardSize > 0)
    return board[cell->index() - 1];
  else if(wrapped)
    return board[cell->index() - 1 + BoardSize];
  else
    return 0;
}

Cell* MainWindow::rCell(Cell* cell) const
{
  if(cell->index() % BoardSize < BoardSize - 1)
    return board[cell->index() + 1];
  else if(wrapped)
    return board[cell->index() + 1 - BoardSize];
  else
    return 0;
}

void MainWindow::lClicked(int index)
{
  rotate(index, true);
}

void MainWindow::rClicked(int index)
{
  rotate(index, false);
}

void MainWindow::rotate(int index, bool toleft)
{
  const Cell::Dirs d = board[index]->dirs();
  if(d == Cell::Free || d == Cell::None || isGameOver())
  {
    if(soundaction->isChecked())
      clicksound->play();
    blink(index);
  }
  else
  {
    if(soundaction->isChecked())
      turnsound->play();
    board[index]->rotate(toleft ? -6 : 6);
    updateConnections();
    for(int i = 0; i < 14; i++)
    {
      qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
      QTimer::singleShot(20, board[index], SLOT(update()));
      qApp->processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents);
      board[index]->rotate(toleft ? -6 : 6);
    }
    if(updateConnections() && soundaction->isChecked())
      connectsound->play();
    lcd->display(lcd->intValue() + 1);
    if(isGameOver())
    {
      if(soundaction->isChecked())
        winsound->play();
      blink(index);    
      if(lcd->intValue() <= highscores.at(2 * (skill + 1) * NumHighscores - 1).toInt())
        addHighscore(lcd->intValue());
                   QMessageBox::information(this,tr("Game Over"),tr("Congratulations! Score %1").arg(lcd->intValue()));
    }
  }
}

void MainWindow::blink(int index)
{
  for(int i = 0; i < board[index]->width() * 2; i += 2)
  {
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    QTimer::singleShot(20, board[index], SLOT(update()));
    qApp->processEvents(QEventLoop::ExcludeUserInputEvents | QEventLoop::WaitForMoreEvents);
    board[index]->setLight(i);
  }
  board[index]->setLight(0);
}

bool MainWindow::isGameOver()
{
  for(int i = 0; i < BoardSize * BoardSize; i++)
  {
    const Cell::Dirs d = board[i]->dirs();
    if(d != Cell::Free && d != Cell::None && !board[i]->isConnected())
      return false;
  }

  return true;
}

void MainWindow::showHighscores()
{
  addHighscore(0);
}

void MainWindow::addHighscore(int score)
{
  QDialog* dialog = new QDialog(this);
  dialog->setWindowTitle(tr("Highscores"));

  QGridLayout* grid = new QGridLayout(dialog);
  grid->setColumnStretch(2, 1);
  grid->setRowStretch(NumHighscores + 3, 1);
  grid->setHorizontalSpacing(40);

  QLabel* label = new QLabel(dialog);
  label->setPixmap(QPixmap(":/computer2.png"));
  grid->addWidget(label, 0, 0, 2, 0);
  label = new QLabel(tr("<h3>Highscores"), dialog);
  grid->addWidget(label, 0, 1, 1, 3);

  const QString header = (skill == Novice) ? tr("Novices") :
    (skill == Normal) ? tr("Amateurs") :
      (skill == Expert) ? tr("Experts") : tr("Masters");

  grid->addWidget(new QLabel("<b>#", dialog), 1, 1);
  grid->addWidget(new QLabel("<b>" + header, dialog), 1, 2);
  grid->addWidget(new QLabel(tr("<b>Scores"), dialog), 1, 3);

  QFrame* frame = new QFrame(dialog);
  frame->setFrameStyle(QFrame::HLine | QFrame::Sunken);
  grid->addWidget(frame, 2, 1, 1, 3);

  QLineEdit* line = 0;
  QStringList::Iterator inserted;
  QStringList::Iterator it = highscores.begin()+(2 * skill * NumHighscores);
  for(unsigned int i = 0; i < NumHighscores; i++)
  {
    label = new QLabel(QString::number(i + 1), dialog);
    grid->addWidget(label, i + 3, 1);

    QStringList::Iterator next = it;
    if(score > 0 && score <= (*(++next)).toInt() && !line)
    {
      inserted = it;
      line = new QLineEdit(username, dialog);
      grid->addWidget(line, i + 3, 2);
      label = new QLabel(QString::number(score), dialog);
      grid->addWidget(label, i + 3, 3);
    }
    else
    {
      grid->addWidget(new QLabel(*(it++), dialog), i + 3, 2);
      grid->addWidget(new QLabel(*(it++), dialog), i + 3, 3);
    }
  }
  QPushButton* button = new QPushButton("OK", dialog);
  connect(button, SIGNAL(clicked()), dialog, SLOT(accept()));
  const int pos = NumHighscores + 4;
  grid->addWidget(button, pos, 1, pos, 3, Qt::AlignHCenter);

  dialog->exec();
  if(line)
  {
    username = line->text();
    *inserted = username;
    *(++inserted) = QString::number(score);
  }
  delete dialog;
}

void MainWindow::setSkill(int s)
{
  skill = s;

  wrapped = false;

  switch (skill) {
    case Master: BoardSize = MasterBoardSize; wrapped = true; break;
    case Expert: BoardSize = ExpertBoardSize; break;
    case Normal: BoardSize = NormalBoardSize; break;
    case Novice: BoardSize = NoviceBoardSize; break;
  }
  initCells();
  newGame();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QSettings settings("QNetWalk", "QNetWalk");
  settings.beginGroup("QNetWalk");
  settings.setValue("Skill", skill);
  settings.setValue("Username", username);
  settings.setValue("Highscores", highscores);
  settings.setValue("Sound", soundaction->isChecked());
  settings.endGroup();

  event->accept();
}

void MainWindow::openHomepage()
{
  if(!startBrowser("http://qt.osdn.org.ua/qnetwalk.html"))
    QMessageBox::warning(this, tr("Error"),
              tr("Could not launch your web browser.\n"
              "Please, check the BROWSER environment's variable."));
}

bool MainWindow::startBrowser(const QString& url)
{
  QStringList browsers;
  QString env = getenv("BROWSER");
  if(!env.isEmpty())
    browsers << env;
  browsers << "konqueror" << "mozilla" << "opera" << "netscape";

  QStringList arguments;
  arguments << url;
  QProcess process;
  while(!browsers.isEmpty())
  {
    if(process.startDetached(browsers.first(), arguments))
      return true;
    browsers.removeFirst();
  }

  return false;
}

void MainWindow::help()
{
  QMessageBox box(this);
  box.setWindowTitle(tr("Rules of Play"));
  box.setIconPixmap(QPixmap(":/computer2.png"));
  box.setText(tr("<h3>Rules of Play</h3>"
        "<p>You are the system administrator and your goal"
        " is to connect each computer to the central server."
        "<p>Click the right mouse's button for turning the cable"
        " in a clockwise direction, and left mouse's button"
        " for turning the cable in a counter-clockwise direction."
        "<p>Start the LAN with as few turns as possible!"));
  box.exec();
}

void MainWindow::about()
{
  QMessageBox box(this);
  box.setWindowTitle(tr("About QNetWalk"));
  box.setIconPixmap(QPixmap(":/computer2.png"));
  box.setText(tr("<h3>About QNetWalk 1.2</h3>"
        "<p>QNetWalk is a free Qt-version of the NetWalk game."
        "<p>Copyright (C) 2004, Andi Peredri"
        "<p>Homepage: http://qt.osdn.org.ua/qnetwalk.html"
        "<p>This program is distributed under the terms of the"
        " GNU General Public License."));
  box.exec();
}

void MainWindow::doResize()
{
  QWidget *w = centralWidget();
  if (w->layout()==0) return;

  if (w->width() > w->height()) {
    w->setContentsMargins((w->width()-w->height())/2,0,
                          (w->width()-w->height())/2,0);
  } else {
    w->setContentsMargins(0,(w->height()-w->width())/2,
                          0,(w->height()-w->width())/2);
  }
}

void MainWindow::resizeEvent(QResizeEvent *)
{
  if (centralWidget()==0) return;

  doResize();
}

