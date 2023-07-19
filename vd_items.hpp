#pragma once

namespace vd::items {

class text final : public QGraphicsTextItem {
public:
	text( const QPointF & pos ) {
		setPos( pos );
		setFont( QFont( "Times", 72/*, QFont::Bold*/ ) );
		setTextInteractionFlags( Qt::TextSelectableByKeyboard | Qt::TextEditable );
		setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
		//p_text->setToolTip( "Test text tooltip" );
		setPlainText( "Текст" );
	}

	void change_size_by( int points ) {
		QFont f = font();
		f.setPointSize( f.pointSize() + points );
		setFont( f );
	}
}; // class text


class size : public QGraphicsItem{
	qint8 qtyofpointSet ; //quantity of set points 1,2 or 3
	qreal x1, y1; //first point  
	qreal x2, y2; //second point
	qreal L; // user space length		
	public:
	size (qreal _x1, qreal _y1): x1(_x1), y1(_y1),qtyofpointSet(1){}//qtyofpointSet = 1 immediately
    	QRectF boundingRect() const override{        
	        return QRectF(0, 0,0,0);
	}
	void setqtyofPoint(qint8 s) {
		qtyofpointSet = s;
	}
	void setX2(qreal _x2) {x2=_x2;}
	void setY2(qreal _y2) {y2=_y2;}
	void setL(qreal  _L)   {L=_L;}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) override{
		switch  ( qtyofpointSet ) {
			case  1:	
			painter->drawEllipse(x1,y1,5,5);
			break;
			case  2:	
			painter->drawEllipse(x1,y1,5,5);
			painter->drawEllipse(x2,y2,15,15);
			break;
		}			
    }
};


} // namespace vd::items
