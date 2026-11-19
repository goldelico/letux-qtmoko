/****************************************************************************
**
** This file is part of the Qt Extended Opensource Package.
**
** Copyright (C) 2009 Trolltech ASA.
**
** Contact: Qt Extended Information (info@qtextended.org)
**
** This file may be used under the terms of the GNU General Public License
** version 2.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.
**
** Please review the following information to ensure GNU General Public
** Licensing requirements will be met:
**     http://www.fsf.org/licensing/licenses/info/GPLv2.html.
**
**
****************************************************************************/

#include <QRect>
#include <QRegion>
#include <QMutex>
#include <QEvent>
#include <QTimer>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QWSEmbedWidget>
#include <QPointer>
#include <QThread>

#include "qtopiavideo.h"
#include "qvideosurface.h"
#include <QDirectPainter>
#include "qabstractvideooutput.h"
#include "qvideooutputloader.h"

//#define VIDEO_SURFACE_GEOMETRY_DEBUG
//#define VIDEO_SURFACE_PAINT_DEBUG

// Shared mutex for video-surface state across instances.
Q_GLOBAL_STATIC(QMutex, qt_video_surface_mutex);

class QVideoSurfacePrivate
{
public:
    QVideoSurfacePrivate(QVideoSurface *parent)
        :q(parent),
         videoOutput(0),
         surface(0),
         mutex(0),
         disableUpdates(false)
    {
    }

    QVideoSurface *q;
    QAbstractVideoOutput *videoOutput;
    QDirectPainter *surface;
    QScreen *screen;
    QPointer< QWSEmbedWidget > videoWidget;

    QRegion prevClipRegion;

    QMutex *mutex;
    bool disableUpdates;

    QRegion requestedRegion;
    QRegion dirtyRegion;
    QTimer dirtyRegionUpdateTimer;

    QVideoFormatList expectedFormats;

    QVideoFormatList supportedFormats;
    QVideoFormatList preferredFormats;
    QtopiaVideo::VideoScaleMode scaleMode;
    QtopiaVideo::VideoRotation rotation;


};


// QDirectPainter is a public Qt/Embedded API.  Its regionChanged() callback
// replaces the private QWS event filter used by QWSDirectPainterSurface.
class QtopiaVideoDirectPainter : public QDirectPainter
{
public:
    explicit QtopiaVideoDirectPainter(QVideoSurface *owner)
        : QDirectPainter(owner, QDirectPainter::ReservedSynchronous), q(owner) {}

    virtual void regionChanged(const QRegion &)
    {
        QMetaObject::invokeMethod(q, "updateVideoOutputClipRegion",
                                  Qt::QueuedConnection);
    }
private:
    QVideoSurface *q;
};

QVideoSurface::QVideoSurface( QObject *parent, const QVideoFormatList& expectedFormats )
    :QObject(parent)
{
    d = new QVideoSurfacePrivate(this);
    d->mutex = qt_video_surface_mutex();

    d->expectedFormats = expectedFormats;

    d->surface = new QtopiaVideoDirectPainter(this);

    d->screen = qt_screen;
    if ( !d->screen->subScreens().isEmpty() ) {
        d->screen = d->screen->subScreens()[0];
    }

    d->videoOutput = QVideoOutputLoader::instance()->create( d->screen, expectedFormats, this );
    d->preferredFormats = d->videoOutput->preferredFormats();
    d->supportedFormats = d->videoOutput->supportedFormats();
    d->scaleMode = d->videoOutput->scaleMode();
    d->rotation = d->videoOutput->rotation();

    moveToThread( qApp->thread() );

    d->dirtyRegionUpdateTimer.setSingleShot(true);
    connect( &d->dirtyRegionUpdateTimer, SIGNAL(timeout()),
             SLOT(updateDirtyRegion()) );

    QVideoOutputLoader::instance()->load();
}

QVideoSurface::~QVideoSurface()
{
    d->mutex->lock();
    delete d->videoOutput;
    d->videoOutput = 0;
    d->disableUpdates = true;
    d->mutex->unlock();

    //force server to repaint the region
    d->dirtyRegion |= d->surface->allocatedRegion();
    updateDirtyRegion();

    d->surface->setRegion( QRegion() );

    delete d->surface;
    delete d;
}


int QVideoSurface::winId() const
{
    return d->surface->winId();
}


void QVideoSurface::setExpectedFormats( const QVideoFormatList& expectedFormats )
{
    if ( d->expectedFormats != expectedFormats ) {
        QMutexLocker locker( d->mutex );
        d->expectedFormats = expectedFormats;
        reloadVideoOutput();
    }
}


QWidget *QVideoSurface::videoWidget()
{
    if (!d->videoWidget)
        d->videoWidget = new QWSEmbedWidget( winId() );

    return d->videoWidget;
}

QtopiaVideo::VideoRotation QVideoSurface::rotation() const
{
    return d->rotation;
}

QtopiaVideo::VideoScaleMode QVideoSurface::scaleMode() const
{
    return d->scaleMode;
}

QRect QVideoSurface::geometry() const
{
    return d->requestedRegion.boundingRect();
}

QRegion QVideoSurface::requestedRegion() const
{
    return d->requestedRegion;
}

QVideoFormatList QVideoSurface::supportedFormats() const
{
    return d->supportedFormats;
}

QVideoFormatList QVideoSurface::preferredFormats() const
{
    if ( !d->videoOutput ) {
        QMutexLocker locker( d->mutex );
    }
    return d->preferredFormats;
}

void QVideoSurface::renderFrame( const QVideoFrame& frame )
{
    QMutexLocker locker(d->mutex);
    if (d->disableUpdates || !d->videoOutput)
        return;

    // Public QDirectPainter handles display locking and QWS region changes.
    d->surface->startPainting();
    if (!d->disableUpdates)
        d->videoOutput->renderFrame(frame);
    d->surface->endPainting();
}

void QVideoSurface::setRotation( QtopiaVideo::VideoRotation rotation )
{
    bool changed = false;
    d->mutex->lock();
    if ( d->rotation != rotation ) {
        d->rotation = rotation;
        d->videoOutput->setRotation( rotation );
        changed = true;
    }
    d->mutex->unlock();

    if ( changed )
        emit updateRequested();
}

void QVideoSurface::setScaleMode( QtopiaVideo::VideoScaleMode scaleMode )
{
    bool changed = false;
    d->mutex->lock();
    if ( d->scaleMode != scaleMode ) {
        d->scaleMode = scaleMode;
        d->videoOutput->setScaleMode( scaleMode );
        changed = true;
    }
    d->mutex->unlock();

    if ( changed )
        emit updateRequested();
}

void QVideoSurface::setGeometry( const QRect &r )
{
    setRegion( QRegion(r) );
}

void QVideoSurface::setRegion( const QRegion& region )
{
    QMutexLocker locker( d->mutex );

    if ( d->requestedRegion == region )
        return;

    d->disableUpdates = true;

#ifdef VIDEO_SURFACE_GEOMETRY_DEBUG
    qWarning() << "QVideoSurface::setRegion" << region.boundingRect() << ";   prev region:" << d->requestedRegion.boundingRect();
#endif

    d->requestedRegion = region;

    if ( !region.isEmpty() ) {
        //are we still on the same screen?
        int screenNum = qt_screen->subScreenIndexAt( region.boundingRect().center() );
        QScreen *screen = screenNum<0 ? qt_screen : qt_screen->subScreens()[screenNum];

        if ( screen != d->screen ) {
#ifdef VIDEO_SURFACE_GEOMETRY_DEBUG
            qWarning() << "Screen changed";
#endif
            d->screen = screen;
            reloadVideoOutput();
        }
    }

    //d->videoOutput->setClipRegion( QRegion() );
    d->videoOutput->setRegion( region );
    d->surface->setRegion(region);
    QTimer::singleShot(0, this, SLOT(updateVideoOutputClipRegion()));
}


void QVideoSurface::reloadVideoOutput()
{
    delete d->videoOutput;
    d->videoOutput = 0;

    d->videoOutput = QVideoOutputLoader::instance()->create( d->screen, d->expectedFormats, this );
    d->videoOutput->setScaleMode( scaleMode() );
    d->videoOutput->setRotation( rotation() );

    if ( d->videoOutput->supportedFormats() != d->supportedFormats ||
         d->videoOutput->preferredFormats() != d->preferredFormats ) {
        d->preferredFormats = d->videoOutput->preferredFormats();
        d->supportedFormats = d->videoOutput->supportedFormats();
        emit formatsChanged();
    }
}


void QVideoSurface::updateVideoOutputClipRegion()
{
    QRegion r = d->surface->allocatedRegion();
    QRegion oldRegion = d->videoOutput->clipRegion();
    if (r != oldRegion) {
        QMutexLocker locker(d->mutex);
        d->videoOutput->setClipRegion(r);
    }
    d->disableUpdates = r.isEmpty();
    emit clipRegionChanged(r);
    if (!(r - d->prevClipRegion).isEmpty())
        emit updateRequested();
    d->prevClipRegion = r;
}

void QVideoSurface::updateDirtyRegion()
{
    // QDirectPainter releases its reserved region through the public QWS API.
    d->dirtyRegion = QRegion();
}

/*!
    Raises the reserved region to the top of the widget stack.

    After this call the reserved region will be visually in front of
    any overlapping widgets.

    \sa lower(), requestedRegion()
*/
void QVideoSurface::raise()
{
    d->surface->raise();
}

/*!
    Lowers the reserved region to the bottom of the widget stack.

    After this call the reserved region will be visually behind (and
    therefore obscured by) any overlapping widgets.

    \sa raise(), requestedRegion()
*/
void QVideoSurface::lower()
{
    d->surface->lower();
}


