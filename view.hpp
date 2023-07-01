﻿#pragma once

#include "VectorDraw_pch.hpp"

class graphics_view : public QGraphicsView {
	Q_OBJECT

	const int			m_scale_factor_min	= 1;
	const int			m_scale_factor_max	= 200;
	const int			m_scale_factor_div	= 10;
	int					m_scale_factor		= m_scale_factor_div;

	QGraphicsRectItem *	m_scene_rect;

	QGraphicsItem *		m_item				= nullptr;		// Current painting item

	void update_zoom() {
		QTransform tform;
		qreal scale = qreal(m_scale_factor) / m_scale_factor_div;
		tform.scale( scale, scale );
		setTransform( tform );
	}

public:
	graphics_view( QGraphicsScene * p_scene, QWidget * p_parent )
		: QGraphicsView( p_scene, p_parent )
	{
		//setDragMode( QGraphicsView::ScrollHandDrag ); // Remove. Moved to keyPressEvent/keyReleaseEvent
		setInteractive( true );
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		m_scene_rect = scene()->addRect( sceneRect(), Qt::NoPen, QBrush( QColor( 255, 255, 255 ) ) );
		m_scene_rect->setFlag( QGraphicsItem::ItemStacksBehindParent );

		update_zoom();
	}

	void set_canvas_size( const QSize & size ) {
		int w = size.width();
		int h = size.height();
		m_scene_rect->setRect( -w / 2, -h / 2, w, h );
	}

protected:

	// Keyboard events...
	void keyPressEvent( QKeyEvent * p_event ) override {
		QGraphicsView::keyPressEvent( p_event ); // Forward to base

		qDebug() << p_event->modifiers();
		if ( p_event->modifiers() & Qt::ControlModifier ) {
			setDragMode( QGraphicsView::ScrollHandDrag );
		}
	}

	void keyReleaseEvent( QKeyEvent * p_event ) override {
		QGraphicsView::keyReleaseEvent( p_event ); // Forward to base

		qDebug() << p_event->modifiers();
		if ( ~(p_event->modifiers() & Qt::ControlModifier) ) {
			setDragMode( QGraphicsView::NoDrag );
		}
	}


	// Mouse events...
	void wheelEvent( QWheelEvent * p_event ) override {
		//QGraphicsView::wheelEvent( p_event ); // I don't need default QGraphicsView wheel behaviour
		if ( p_event->modifiers() & Qt::ControlModifier ) {
			m_scale_factor += p_event->angleDelta().y() > 0 ? 1 : -1;
			m_scale_factor = std::clamp( m_scale_factor, m_scale_factor_min, m_scale_factor_max );
			update_zoom();
		}
	}

	void mousePressEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mousePressEvent( p_event ); // Forward to base

		// TODO: check selected tool
		QPointF pt = mapToScene( p_event->pos() );
		m_item = scene()->addLine( QLineF( pt, pt ) );
	}

	void mouseReleaseEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mouseReleaseEvent( p_event ); // Forward to base

		m_item = nullptr; // Stop drawing the item
	}

	void mouseMoveEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mouseMoveEvent( p_event ); // Forward to base

		// TODO: check selected tool
		if ( m_item ) {
			QGraphicsLineItem * p = static_cast<QGraphicsLineItem *>( m_item );
			p->setLine( QLineF( p->line().p1(), mapToScene( p_event->pos() ) ) );
		}
	}

	void scrollContentsBy( int dx, int dy ) override {
		QGraphicsView::scrollContentsBy( dx, dy ); // Forward to base
	}
}; // class graphics_view
