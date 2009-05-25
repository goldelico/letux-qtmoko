/*******************************************************************
 *
 * Copyright (C) 2006 Dmitry Suzdalev <dimsuz@gmail.com>
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
#ifndef RENDERER_H
#define RENDERER_H

#include <QtSvg/QSvgRenderer>


#include <QObject>
#include <QSize>
#include <QHash>
#include <QPixmap>

namespace QSame
{
	/**
	 * Class for rendering game elements from SVG
	 */
	class Renderer: public QObject
	{
		Q_OBJECT
		public:
			/**
			 * Constructor.
			 * @param fileName name of the SVG file containing the game elements
			 */
			explicit Renderer( const QString& fileName,
			          const QSize& backgroundSize = QSize( 300, 400 ),
			          const QSize& elementSize = QSize( 64, 64 ),
			          QObject* parent = 0 );
			/**
			 * Destructor.
			 */
			~Renderer();

			/**
			 * Sets the size of the game elements (stones) and clears the cache.
			 */
			void setElementSize( const QSize& size );
			/**
			 * Sets the background size and clears the cache.
			 */
			void setBackgroundSize( const QSize& size );

			/**
			 * Renders an element from the SVG to a pixmap.
			 * The size of the pixmap will be the one set by
			 * the last call to setElementSize().
			 */
			QPixmap renderElement( const QString& elementId );
			QPixmap renderHighlightedElement( const QString& elementId );
			QPixmap renderCursorElement( const QString& elementId );
			/**
			 * Renders the background to a pixmap.
			 * The size of the pixmap will be the one set by
			 * the last call to setBackgroundSize().
			 */
			QPixmap renderBackground();

		private:
			// The SVG Renderer
			QSvgRenderer m_svgRenderer;
			// Elements' sizes
			QSize        m_backgroundSize;
			QSize        m_elementSize;
			// Caches
			QPixmap      m_cachedBackground;
			QHash<QString, QPixmap> m_elementCache;
			QHash<QString, QPixmap> m_highlightedElementCache;
			QHash<QString, QPixmap> m_cursorElementCache;
	};
} // namespace QSame

#endif // RENDERER_H
