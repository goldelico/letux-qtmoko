//  qnetwalk/cell.cpp
//  Copyright (C) 2004, Andi Peredri <andi@ukr.net>
//  Ported to Qt4 by xep

#include "cell.h"

#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QMouseEvent>
#include <QPaintEvent>

Cell::PixmapMap Cell::connectedpixmap;
Cell::PixmapMap Cell::disconnectedpixmap;

void Cell::initPixmaps()
{
  typedef QMap<int, QString> NamesMap;
  NamesMap names;
  names[L]     = "0001";
  names[D]     = "0010";
  names[D|L]   = "0011";
  names[R]     = "0100";
  names[R|L]   = "0101";
  names[R|D]   = "0110";
  names[R|D|L] = "0111";
  names[U]     = "1000";
  names[U|L]   = "1001";
  names[U|D]   = "1010";
  names[U|D|L] = "1011";
  names[U|R]   = "1100";
  names[U|R|L] = "1101";
  names[U|R|D] = "1110";

  NamesMap::ConstIterator it;
  for(it = names.constBegin(); it != names.constEnd(); ++it)
  {
    connectedpixmap[it.key()] = new QPixmap(":/cable" + it.value() + ".png");

    QImage image = connectedpixmap[it.key()]->toImage();
    for(int y = 0; y < image.height(); y++)
    {
      QRgb* line = (QRgb*)image.scanLine(y);
      for(int x = 0; x < image.width(); x++)
      {
        QRgb pix = line[x];
        if(qAlpha(pix) == 255)
        {
          int g = (255 + 4 * qGreen(pix)) / 5;
          int b = (255 + 4 * qBlue(pix)) / 5;
          int r = (255 + 4 * qRed(pix)) / 5;
          line[x] = qRgb(r, g, b);
        }
      }
    }

    disconnectedpixmap[it.key()] = new QPixmap(QPixmap::fromImage(image));
  }
}


Cell::Cell(QWidget* parent, int i) : QWidget(parent)
{
  angle = 0;
  light = 0;
  iindex = i;
  ddirs = Free;
  changed = true;
  connected = false;
  root = false;
  selected = false;
}

int Cell::index() const
{
  return iindex;
}

Cell::Dirs Cell::dirs() const
{
  return ddirs;
}

bool Cell::isConnected() const
{
  return connected;
}

bool Cell::isRotated() const
{
  return angle;
}

void Cell::setDirs(Dirs d)
{
  if(ddirs == d)
    return;
  ddirs = d;
  changed = true;
  update();
}

void Cell::setConnected(bool b)
{
  if(connected == b)
    return;
  connected = b;
  changed = true;
  update();
}

void Cell::setRoot(bool b)
{
  if(root == b)
    return;
  root = b;
  changed = true;
  update();
}

void Cell::setLight(int l)
{
  light = l;
  changed = true;
  update();
}

void Cell::setSelected(bool b) 
{
  if (selected!=b) {
    selected = b;
    changed = true;
    update();
  }
}

bool Cell::isSelected() 
{
  return selected;
}

void Cell::paintEvent(QPaintEvent*)
{
  if(changed)
  {
    changed = false;
    if (!selected)
      pixmap = QPixmap(":/background.png");
    else
      pixmap = QPixmap(":/backgrounds.png");
       
    QPainter paint;
    paint.begin(&pixmap);

    if(light)
    {
      paint.setPen(QPen(Qt::white, 5));
      paint.drawLine(0, width() - light, width(), 2 * width() - light);
    }

    if(ddirs != Free && ddirs != None)
    {
      int offset = 0;
      if(angle)
      {
        offset = pixmap.width() / 2;
        paint.translate(offset, offset);
        paint.rotate(angle);
      }

      if(connected)
        paint.drawPixmap(-offset, -offset, *connectedpixmap[ddirs]);
      else paint.drawPixmap(-offset, -offset, *disconnectedpixmap[ddirs]);
        paint.resetMatrix();

      if(root)
      {
        paint.drawPixmap(0, 0, QPixmap(":/server.png"));
      }
      else if(ddirs == U || ddirs == L || ddirs == D || ddirs == R)
      {
        if(connected)
          paint.drawPixmap(0, 0, QPixmap(":/computer2.png"));
        else
          paint.drawPixmap(0, 0, QPixmap(":/computer1.png"));
      }
    }
    paint.end();
  }
        
  QPainter paint(this);
  paint.drawPixmap(0, 0, width(), height(), pixmap);
//  paint.drawPixmap(width()-16, 0 ,16,16, QPixmap(":/rotate_cw.png"));
//  paint.drawPixmap(0, height()-16,16,16, QPixmap(":/rotate_ccw.png"));
}

void Cell::mousePressEvent(QMouseEvent* e)
{
  /*if(e->button() == Qt::LeftButton)
    emit lClicked(iindex);
  else if(e->button() == Qt::RightButton)
    emit rClicked(iindex);
  */
  if (e->x()<e->y() )
    emit lClicked(iindex);
  else
    emit rClicked(iindex);
}

void Cell::rotate(int a)
{
  angle += a;
  changed = true;
  while(angle >= 45)
  {
    angle -= 90;
    int newdirs = Free;
    if(ddirs & U)
      newdirs |= R;
    if(ddirs & R)
      newdirs |= D;
    if(ddirs & D)
      newdirs |= L;
    if(ddirs & L)
      newdirs |= U;
    setDirs(Dirs(newdirs));
  }
  while(angle < -45)
  {
    angle += 90;
    int newdirs = Free;
    if(ddirs & U)
      newdirs |= L;
    if(ddirs & R)
      newdirs |= U;
    if(ddirs & D)
      newdirs |= R;
    if(ddirs & L)
      newdirs |= D;
    setDirs(Dirs(newdirs));
  }
  update();
}
