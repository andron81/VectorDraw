#pragma once
#include "vd_view_canvas.hpp"

namespace vd {


class view : public QGraphicsView {
	const int			m_scale_factor_min		= 1;
	const int			m_scale_factor_max		= 200;
	const int			m_scale_factor_div		= 10;
	int					m_scale_factor			= m_scale_factor_div;
	QWidget * main_win;
	vd::view_canvas *	m_canvas;
	painter	m_painter;
	//QGraphicsItem *		m_focus					= nullptr;	// Focused item
	//QPen				m_focus_line_pen_prev	= Qt::NoPen;


	void update_zoom() {
		QTransform tform;
		qreal scale = qreal(m_scale_factor) / m_scale_factor_div;
		tform.scale( scale, scale );
		setTransform( tform );
	}

public:
	view( QGraphicsScene * p_scene, QWidget * p_parent );
		
	painter get_m_painter() {return m_painter;}		
	const view_canvas * get_canvas() const { return m_canvas; }
	      view_canvas * get_canvas()       { return m_canvas; }

		
	void set_canvas_size( const QSize & size ) {
		m_canvas->set_size( size );
	}

	void set_tool( vd::tool_e tool ) {
		m_painter.set_tool( tool );
	}
	void create_canvas(){
		
			m_canvas = new view_canvas;
			scene()->addItem( m_canvas );
	}


	void save_to_image( const QString & filename, const QString ext ) {
		QRectF rect(m_canvas->get_top_left().x(), m_canvas->get_top_left().y(), scene()->width(), scene()->height());
		QImage image(m_canvas->get_size(), QImage::Format_ARGB32_Premultiplied);
		QPainter painter(&image);
		scene()->render(&painter,rect, QRectF(-m_canvas->get_size().width()
		,-m_canvas->get_size().height(), scene()->width(), scene()->height()));		
		QByteArray ba = ext.toLocal8Bit();
		const char *ext_c = ba.data();		
		image.save(filename,ext_c);		
	}

	void print() {
		QPrinter printer( QPrinter::HighResolution );
		QPainter painter( &printer );
		scene()->render( &painter );
	}

protected:
	void keyReleaseEvent( QKeyEvent * p_event ) override {
		// Disable ScrollHandDrag if Ctrl unpressed
		if ( ~(p_event->modifiers() & Qt::ControlModifier) ) {
			setDragMode( QGraphicsView::NoDrag );
		}

		QGraphicsView::keyReleaseEvent( p_event ); // Forward to base
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

	void mouseReleaseEvent( QMouseEvent * p_event ) override {
		QGraphicsView::mouseReleaseEvent( p_event ); // Forward to base
		m_painter.mouse_release_event( p_event );
	}

	void mouseMoveEvent( QMouseEvent * p_event ) override {
		
		QGraphicsView::mouseMoveEvent( p_event ); // Forward to base
		m_painter.mouse_move_event( p_event );
	}
}; // class view

}; // namespace vd
