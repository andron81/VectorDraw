#pragma once

namespace vd {

// Canvas visualization rect.
class view_canvas : public QGraphicsRectItem {
public:
	view_canvas() {
		setFlag( QGraphicsItem::ItemStacksBehindParent );
		setPen( Qt::NoPen );
		setBrush( QColor( 255, 255, 255 ) );

		QGraphicsDropShadowEffect * p_effect = new QGraphicsDropShadowEffect;
		p_effect->setBlurRadius( 32.f );
		p_effect->setOffset( 0 );
		p_effect->setColor( QColor( 64, 64, 64 ) );
		setGraphicsEffect( p_effect );
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
}; // class view_canvas

}; // namespace vd
