#pragma once

#include "vd_view_painter.hpp"

namespace vd {

class view : public QGraphicsView {
	//Q_OBJECT

	const int			m_scale_factor_min	= 1;
	const int			m_scale_factor_max	= 200;
	const int			m_scale_factor_div	= 10;
	int					m_scale_factor		= m_scale_factor_div;

	vd::view_canvas *	m_canvas;
	painter				m_painter;

	QGraphicsItem *		m_focus				= nullptr;	// Focused item

	void update_zoom() {
		QTransform tform;
		qreal scale = qreal(m_scale_factor) / m_scale_factor_div;
		tform.scale( scale, scale );
		setTransform( tform );
	}

public:
	view( QGraphicsScene * p_scene, QWidget * p_parent )
		: QGraphicsView( p_scene, p_parent )
		, m_painter( this )
	{
		//setInteractive( true );
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		m_canvas = new view_canvas;
		scene()->addItem( m_canvas );

		connect( scene(), &QGraphicsScene::focusItemChanged, this,
			[&]( QGraphicsItem * p_new, QGraphicsItem * p_old, Qt::FocusReason reason ) {
				qDebug() << "new: " << p_new << ", old: " << p_old;
				m_focus = p_new;
			} );

		update_zoom();
	}

	const view_canvas * get_canvas() const { return m_canvas; }
	      view_canvas * get_canvas()       { return m_canvas; }

	void set_canvas_size( const QSize & size ) {
		m_canvas->set_size( size );
	}

	void set_tool( tool_e tool ) {
		qDebug() << __FUNCTION__ << ": " << int(tool);

		QGraphicsView::DragMode drag_mode = QGraphicsView::NoDrag;

		//enum class tool_e { none, edit, line_solid, line_dashed, text, size };
		switch ( tool ) {
			case tool_e::edit:
				drag_mode = QGraphicsView::RubberBandDrag;
				break;

			case tool_e::line_solid: [[fallthrough]];
			case tool_e::line_dashed:
				//drag_mode = QGraphicsView::RubberBandDrag;
				break;
		}

		setDragMode( drag_mode );

		m_painter.set_tool( tool );
	}

protected:

	QGraphicsView::DragMode	m_prev_drag_mode = QGraphicsView::NoDrag;

	// Keyboard events...
	void keyPressEvent( QKeyEvent * p_event ) override {
		//QGraphicsView::keyPressEvent( p_event ); // Forward to base

		if ( m_focus ) {

			int offset = p_event->modifiers() & Qt::ShiftModifier ? 10 : 1;
			switch ( p_event->key() /*int*/ ) {
				case Qt::Key_Up:	m_focus->moveBy(       0, -offset ); break;
				case Qt::Key_Left:	m_focus->moveBy( -offset,       0 ); break;
				case Qt::Key_Right:	m_focus->moveBy(  offset,       0 ); break;
				case Qt::Key_Down:	m_focus->moveBy(       0,  offset ); break;
			}
		}

		// Enable ScrollHandDrag if Ctrl pressed
		if ( p_event->modifiers() & Qt::ControlModifier ) {
			m_prev_drag_mode = dragMode();
			setDragMode( QGraphicsView::ScrollHandDrag );
		}
	}

	void keyReleaseEvent( QKeyEvent * p_event ) override {
		//QGraphicsView::keyReleaseEvent( p_event ); // Forward to base

		// Disable ScrollHandDrag if Ctrl unpressed
		if ( ~(p_event->modifiers() & Qt::ControlModifier) ) {
			setDragMode( m_prev_drag_mode );
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

		// Process painter's mouse button event only if NoDrag
		if ( dragMode() == QGraphicsView::NoDrag ) {
			m_painter.mouse_press_event( p_event );
		}
	}

#if 0
	void mouseReleaseEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mouseReleaseEvent( p_event ); // Forward to base
		m_painter.mouse_release_event( p_event );
	}
#endif

	void mouseMoveEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mouseMoveEvent( p_event ); // Forward to base
		m_painter.mouse_move_event( p_event );
	}

	void paintEvent( QPaintEvent * p_event ) override {
		QGraphicsView::paintEvent( p_event ); // Forward to base

		if ( m_focus ) {
			QPointF pos = m_focus->pos();

			if ( m_focus->type() == QGraphicsLineItem::Type ) {

				QGraphicsLineItem * p_line = qgraphicsitem_cast<QGraphicsLineItem *>( m_focus );

				QPointF p1 = pos + p_line->line().p1();
				QPointF p2 = pos + p_line->line().p2();

				qreal r = 5.5f;
				QPainter painter( viewport() );
				painter.setRenderHint( QPainter::Antialiasing );
				painter.setPen( QColor( 255, 0, 0, 127 ) );
				painter.setBrush( QBrush( QColor( 0, 255, 0, 64 ) ) );
				painter.drawEllipse( QPointF( mapFromScene( p1 ) ), r, r );
				painter.drawEllipse( QPointF( mapFromScene( p2 ) ), r, r );
			}
		}

		//qDebug() << "*";
	}

#if 0
	void scrollContentsBy( int dx, int dy ) override {
		QGraphicsView::scrollContentsBy( dx, dy ); // Forward to base
	}
#endif
}; // class view

} // namespace vd
