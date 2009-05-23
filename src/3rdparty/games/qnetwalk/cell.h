//  qnetwalk/cell.h
//  Copyright (C) 2004, Andi Peredri <andi@ukr.net>
//  Ported to Qt4 by xep

#ifndef CELL_H
#define CELL_H

#include <QPixmap>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

class Cell : public QWidget
{
	Q_OBJECT

public:
	enum Dirs { Free = 0, U = 1, R = 2, D = 4, L = 8, None = 16 };

	Cell(QWidget* parent, int i);

	int index() const;
	void rotate(int a);
	void setDirs(Dirs d);
	void setRoot(bool b);
	void setLight(int l);
	void setConnected(bool b);
	bool isConnected() const;
	bool isRotated() const;
        bool isSelected();
        void setSelected(bool b);
	Dirs dirs() const;
	static void initPixmaps();

signals:
	void lClicked(int);
	void rClicked(int);

protected:
	virtual void paintEvent(QPaintEvent*);
	virtual void mousePressEvent(QMouseEvent*);

private:
	typedef QMap<int, QPixmap*> PixmapMap;

	static PixmapMap connectedpixmap;
	static PixmapMap disconnectedpixmap;
	int	angle;
	int	light;
	int	iindex;
	bool connected;
	bool changed;
	bool root;
        bool selected;
	Dirs ddirs;
	QPixmap pixmap;
};

#endif
