#pragma once

// Canvas visualization rect.
class canvas : public QGraphicsRectItem {
public:
	canvas() {
		setFlag( QGraphicsItem::ItemStacksBehindParent );
		setBrush( QBrush( QColor( 255, 255, 255 ) ) );
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

	virtual ~canvas() { qDebug() << __FUNCTION__; }
}; // class canvas
