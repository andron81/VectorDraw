#pragma once

namespace vd {

// Canvas visualization rect.
class view_canvas : public QGraphicsRectItem {
public:
	view_canvas() {
		setFlag( QGraphicsItem::ItemStacksBehindParent );
		setPen( Qt::NoPen );
		setBrush( QColor( 255, 255, 255 ) );
	}

	QPointF get_top_left() const {
		return rect().topLeft();
	}

	QSize get_size() const {
		return rect().size().toSize();
	}

	void set_size( const QSize & size ) {
		int w = size.width();
		int h = size.height();
		setRect( -w / 2, -h / 2, w, h );
	}

	virtual ~view_canvas() { qDebug() << __FUNCTION__; }
}; // class view_canvas

} // namespace vd
