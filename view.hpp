#pragma once

#include "VectorDraw_pch.hpp"

class graphics_view : public QGraphicsView {
	Q_OBJECT

	const int			m_scale_factor_min	= 1;
	const int			m_scale_factor_max	= 200;
	const int			m_scale_factor_div	= 10;
	int					m_scale_factor		= m_scale_factor_div;

	QGraphicsRectItem *	m_scene_rect;

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
		setDragMode( QGraphicsView::ScrollHandDrag );
		setInteractive( true );
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		m_scene_rect = scene()->addRect( sceneRect(), Qt::NoPen, QBrush( QColor( 255, 255, 255 ) ) );
		m_scene_rect->setFlag( QGraphicsItem::ItemStacksBehindParent );

		QGraphicsLineItem * p = scene()->addLine( -100, 0, 100, 0, QPen( Qt::DashLine ) );
		p->setAcceptHoverEvents( true );
		p->setAcceptTouchEvents( true );
		p->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );

		update_zoom();
	}

	void set_canvas_size( const QSize & size ) {
		int w = size.width();
		int h = size.height();
		m_scene_rect->setRect( -w / 2, -h / 2, w, h );
	}

protected:
	void wheelEvent( QWheelEvent * p_event ) override {
		//QGraphicsView::wheelEvent( p_event ); // I don't need default QGraphicsView wheel behaviour

		if ( p_event->modifiers() & Qt::ControlModifier ) {
			m_scale_factor += p_event->angleDelta().y() > 0 ? 1 : -1;
			m_scale_factor = std::clamp( m_scale_factor, m_scale_factor_min, m_scale_factor_max );
			update_zoom();
		}
	}

	void mouseMoveEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mouseMoveEvent( p_event ); // Forward to base

#if 0
		if ( QGraphicsItem * p_item = itemAt( p_event->position().toPoint() ) ) {
			//p_item->setFocus();
			p_item->setSelected( true );
		}
#endif
	}

	void scrollContentsBy( int dx, int dy ) override {
		QGraphicsView::scrollContentsBy( dx, dy ); // Forward to base
	}
}; // class graphics_view
