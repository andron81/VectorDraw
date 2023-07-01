#pragma once

#include "VectorDraw_pch.hpp"

class graphics_view : public QGraphicsView {
	Q_OBJECT

	int			m_scale	= 1;

public:
	graphics_view( QGraphicsScene * scene, QWidget * parent )
		: QGraphicsView( scene, parent )
	{
		setDragMode( QGraphicsView::ScrollHandDrag );
		//m_view->setDragMode( QGraphicsView::RubberBandDrag );
		setInteractive( true );
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
		//setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		QTransform transform;
		transform.scale( m_scale, m_scale );
		//qreal shift = 0.5 / scale;
		//transform.translate(shift, shift);
		setTransform( transform );
	}

protected:
	void wheelEvent( QWheelEvent * event ) override {
		//qDebug() << __FUNCTION__ << ", angle: " << event->angleDelta().y() << ", pixel: " << event->pixelDelta().y();
		//if ( event->modifiers() & Qt::ShiftModifier )

		if ( event->modifiers() & Qt::ControlModifier ) {
			m_scale += event->angleDelta().y() > 0 ? 1 : -1;
			if ( m_scale <  1 ) m_scale = 1;
			if ( m_scale > 10 ) m_scale = 10;

			QTransform tform;
			tform.scale( m_scale, m_scale );
			setTransform( tform );
		}
	}

	void resizeEvent( QResizeEvent * event ) override { qDebug() << __FUNCTION__; }
	void scrollContentsBy( int dx, int dy ) override {
		//qDebug() << __FUNCTION__;
		QGraphicsView::scrollContentsBy( dx, dy );
	}
}; // class graphics_view
