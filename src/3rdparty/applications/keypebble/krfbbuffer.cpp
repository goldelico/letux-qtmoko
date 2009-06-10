#include <assert.h>
#include <qpixmap.h>
#include <qbrush.h>
#include <qimage.h>
#include <qpainter.h>
#include <qapplication.h>
#include <qevent.h>

#include "krfbdecoder.h"
#include "krfbbuffer.h"
#include "krfbserverinfo.h"

//
// Endian stuff
//
#ifndef KDE_USE_FINAL
const int endianTest = 1;
#endif

#define Swap16IfLE(s) \
    (*(char *)&endianTest ? ((((s) & 0xff) << 8) | (((s) >> 8) & 0xff)) : (s))

#define Swap32IfLE(l) \
    (*(char *)&endianTest ? ((((l) & 0xff000000) >> 24) | \
			     (((l) & 0x00ff0000) >> 8)  | \
			     (((l) & 0x0000ff00) << 8)  | \
			     (((l) & 0x000000ff) << 24))  : (l))


KRFBBuffer::KRFBBuffer( KRFBDecoder *decoder)
{
    assert( decoder );
    this->decoder = decoder;
    buffer = new QImage( 1, 1, QImage::Format_RGB32 );
    scaler = 1;
}

KRFBBuffer::~KRFBBuffer()
{
    delete buffer;
}

void KRFBBuffer::paint( QPainter *p, int x, int y, int w, int h ) const
{
    if ( buffer->isNull() )
	return;
    QRect dr( x, y, w, h );
    dr &= QRect( 0, 0, buffer->width()/scaler, buffer->height()/scaler );

    if ( !dr.isEmpty() ) {
      if ( scaler == 1 ) {
        p->drawImage( 0, 0, *buffer, dr.x(), dr.y(), dr.width(), dr.height(), 0 );
      } else 
        p->drawImage( 0, 0, buffer->scaled(buffer->width()/scaler, buffer->height()/scaler, 
           Qt::IgnoreAspectRatio, Qt::FastTransformation),dr.x(),dr.y(),dr.width(),dr.height(),0);
    }
}

QImage KRFBBuffer::fastScale( QImage &si ) const
{
    QImage di( si.width()/scaler, si.height()/scaler, QImage::Format_RGB32 );

    for ( int y = 0; y < di.height(); y++ ) {
	quint32 *dst = (quint32 *)di.scanLine( y );
	quint32 *src1 = (quint32 *)si.scanLine( y*scaler );
	quint32 *src2 = (quint32 *)si.scanLine( y*scaler+1 );
	for ( int x = 0; x < di.width(); x++ ) {
	    quint32 r = (*src1) & 0x00ff0000;
	    quint32 g = (*src1) & 0x0000ff00;
	    quint32 b = (*src1) & 0x0000ff;
	    r += (*(src1+1)) & 0x00ff0000;
	    g += (*(src1+1)) & 0x0000ff00;
	    b += (*(src1+1)) & 0x000000ff;
	    r += (*src2) & 0x00ff0000;
	    g += (*src2) & 0x0000ff00;
	    b += (*src2) & 0x000000ff;
	    r += (*(src2+1)) & 0x00ff0000;
	    g += (*(src2+1)) & 0x0000ff00;
	    b += (*(src2+1)) & 0x000000ff;
	    *dst = ((r>>2)&0x00ff0000) | ((g>>2)&0x0000ff00) | (b>>2);
	    dst++;
	    src1 += scaler;
	    src2 += scaler;
	}
    }

    return di;
}

void KRFBBuffer::resize( int w, int h )
{
    qWarning( "Resizing buffer:w=%d, h=%d",w,h );

    delete buffer;

    if ( (decoder && decoder->format && decoder->format-> bpp ) && decoder->format->bpp <= 16 ) {
        qWarning( "Resizing buffer: Format RGB16" );
        buffer = new QImage( w, h, QImage::Format_RGB16 );
    } else {
        qWarning( "Resizing buffer: Format RGB32" );
        buffer = new QImage( w, h, QImage::Format_RGB32 );
    }
    emit sizeChanged( w/scaler, h/scaler );
}

void KRFBBuffer::soundBell()
{
    emit bell();
}

void KRFBBuffer::setScaling( int s )
{
    scaler = s;
    emit sizeChanged( buffer->width()/scaler, buffer->height()/scaler );
}

void KRFBBuffer::mouseEvent( QMouseEvent *e )
{
    QMouseEvent te( e->type(), e->pos()*scaler, e->button(), e->buttons(), 0 );
    decoder->sendMouseEvent( &te );
}

void KRFBBuffer::keyPressEvent( QKeyEvent *e )
{
    decoder->sendKeyPressEvent( e );
}

void KRFBBuffer::keyReleaseEvent( QKeyEvent *e )
{
    decoder->sendKeyReleaseEvent( e );
}

void KRFBBuffer::copyRect( int srcX, int srcY,
                           int destX, int destY, int w, int h )
{
    if ( srcY > destY ) {
	for ( int j = 0; j < h; j++ ) {
	    uchar *sp = buffer->scanLine( j+srcY ) + srcX*4;
	    uchar *dp = buffer->scanLine( j+destY ) + destX*4;
	    memmove( dp, sp, w*4 );
	}
    } else {
	for ( int j = h-1; j >= 0; j-- ) {
	    uchar *sp = buffer->scanLine( j+srcY ) + srcX*4;
	    uchar *dp = buffer->scanLine( j+destY ) + destX*4;
	    memmove( dp, sp, w*4 );
	}
    }

    updateDone( destX, destY, w, h );
}

void KRFBBuffer::drawRawRectChunk( void *data,
				   int x, int y, int w, int h )
{
    int redMax = Swap16IfLE( decoder->format->redMax );
    int greenMax = Swap16IfLE( decoder->format->greenMax );
    int blueMax = Swap16IfLE( decoder->format->blueMax );

    if ( decoder->format->bpp == 8 ) {
	uchar *d = (unsigned char *) data;

	uint r,g,b;

	for ( int j = 0; j < h; j++ ) {
	    uint *p = ( uint * ) (buffer->scanLine( j+y ) + x*4 );
	    for ( int i = 0; i < w ; i++, p++ ) {
		uchar sp = d[ j * w + i ];
		r = sp;
		r = r >> decoder->format->redShift;
		r = r & redMax;

		g = sp;
		g = g >> decoder->format->greenShift;
		g = g & greenMax;

		b = sp;
		b = b >> decoder->format->blueShift;
		b = b & blueMax;

		r = ( r * 255 ) / redMax;
		g = ( g * 255 ) / greenMax;
		b = ( b * 255 ) / blueMax;

		*p = qRgb( r,g,b );
	    }
	}
    } else if ( decoder->format->bpp == 16 ) {
	ushort *d = (ushort *) data;

	ulong r,g,b;

	for ( int j = 0; j < h; j++ ) {
	    uint *p = ( uint * ) (buffer->scanLine( j+y ) + x*4 );
	    for ( int i = 0; i < w ; i++, p++ ) {
		ushort pixel = d[ j * w + i ];
                //pixel = Swap16IfLE( pixel );

		r = pixel;
		r = r >> decoder->format->redShift;
		r = r & redMax;

		g = pixel;
		g = g >> decoder->format->greenShift;
		g = g & greenMax;

		b = pixel;
		b = b >> decoder->format->blueShift;
		b = b & blueMax;

		r = ( r * 255 ) / redMax;
		g = ( g * 255 ) / greenMax;
		b = ( b * 255 ) / blueMax;

		*p = qRgb( r,g,b );
	    }
	}
    } else if ( decoder->format->bpp == 8 ) {
	ulong *d = (ulong *) data;

	ulong r,g,b;

	for ( int j = 0; j < h; j++ ) {
	    uint *p = ( uint * ) (buffer->scanLine( j+y ) + x*4 );
	    for ( int i = 0; i < w ; i++, p++ ) {
		ulong pixel = d[ j * w + i ];
		pixel = Swap32IfLE( pixel );

		r = pixel;
		r = r >> decoder->format->redShift;
		r = r & redMax;

		g = pixel;
		g = g >> decoder->format->greenShift;
		g = g & greenMax;

		b = pixel;
		b = b >> decoder->format->blueShift;
		b = b & blueMax;

		r = ( r * 255 ) / redMax;
		g = ( g * 255 ) / greenMax;
		b = ( b * 255 ) / blueMax;

		*p = qRgb( r,g,b );
	    }
	}
    }
    else {
	// just clear it
    }
    updateDone( x, y, w, h );
}

void KRFBBuffer::fillRect( int x, int y, int w, int h, unsigned long p )
{
    int redMax = Swap16IfLE( decoder->format->redMax );
    int greenMax = Swap16IfLE( decoder->format->greenMax );
    int blueMax = Swap16IfLE( decoder->format->blueMax );

    if ( decoder->format->bpp == 8 || decoder->format->bpp == 16 ) {
	uint r = p >> decoder->format->redShift;
	r = r & redMax;
	r = ( r * 255 ) / redMax;

	uint g = p >> decoder->format->greenShift;
	g = g & greenMax;
	g = ( g * 255 ) / greenMax;

	uint b = p >> decoder->format->blueShift;
	b = b & blueMax;
	b = ( b * 255 ) / blueMax;

	ulong pixel = qRgb( r,g,b );
	for ( int j = 0; j < h; j++ ) {
	    uint *p = ( uint * ) (buffer->scanLine( j+y ) + x*4 );
	    for ( int i = 0; i < w ; i++, p++ ) {
		*p = pixel;
	    }
	}
    } else if ( decoder->format->bpp == 32 ) {
	qWarning( "Implement 32bpp fillRect" );
    }
}

void KRFBBuffer::updateDone( int x, int y, int w, int h )
{
    if ( scaler != 1 ) {
	int x2 = (x+w+scaler-1)/scaler;
	int y2 = (y+h+scaler-1)/scaler;
	x /= scaler;
	y /= scaler;
	w = x2-x;
	h = y2-y;
    }

    emit updated( x, y, w, h );
}

