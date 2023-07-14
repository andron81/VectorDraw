#pragma once

namespace vd {

class view : public QGraphicsView {
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
		m_painter.set_tool( tool );
	}

	void save_image() {

		QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить", ".", "Изображения (*.png *.jpg)" );
		if ( filename.isEmpty() ) return;

		QSize  canvas_sz( m_canvas->get_size() );
		QPoint canvas_pt( mapFromScene( m_canvas->get_top_left() ) );

		QImage file( canvas_sz, QImage::Format_RGB32 );
		QPainter painter( &file );
		painter.setRenderHints( renderHints() );

		// Renders the source rect, which is in view coordinates, from the scene into target, which is in paint device coordinates, using painter.
		// This function is useful for capturing the contents of the view onto a paint device, such as a QImage (e.g., to take a screenshot), or for printing to QPrinter.
		// ...
		// If target is a null rect, the full dimensions of painter's paint device (e.g., for a QPrinter, the page size) will be used.
		render( &painter, QRectF() /*target*/, QRect( canvas_pt, canvas_sz ) /*source*/ );

		file.save( filename );
	}

protected:
	// Keyboard events...
	void keyPressEvent( QKeyEvent * p_event ) override {

		// Modify item's properties if Ctrl pressed...
		if ( p_event->modifiers() & Qt::ControlModifier ) {
			setDragMode( QGraphicsView::ScrollHandDrag );

			if ( m_focus ) {

				// All items...
				int modifier_value = p_event->modifiers() & Qt::ShiftModifier ? 10 : 1;
				switch ( p_event->key() ) {

					// Position...
					case Qt::Key_Up:	m_focus->moveBy(               0, -modifier_value ); break;
					case Qt::Key_Left:	m_focus->moveBy( -modifier_value,               0 ); break;
					case Qt::Key_Right:	m_focus->moveBy(  modifier_value,               0 ); break;
					case Qt::Key_Down:	m_focus->moveBy(               0,  modifier_value ); break;
				}

				// Text...
				if ( m_focus->type() == QGraphicsTextItem::Type ) {
					items::text * p_text = qgraphicsitem_cast<items::text *>( m_focus );
					Q_ASSERT( p_text );
					switch ( p_event->key() ) {

						// Size...
						case Qt::Key_Minus:	p_text->change_size_by( -modifier_value ); break;
						case Qt::Key_Plus:	p_text->change_size_by(  modifier_value ); break;
					}
				}

			}
		} else {
			QGraphicsView::keyPressEvent( p_event ); // Forward to base
		}
	}

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

} // namespace vd
