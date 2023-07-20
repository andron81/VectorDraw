#pragma once

namespace vd::items {

enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };

struct item_base {
	virtual int type() const = 0;
	virtual QJsonObject to_JSON() const = 0;
}; // class item


class text final : public item_base, public QGraphicsTextItem {
public:
	enum { Type = types::e_text };

	// Enable the use of qgraphicsitem_cast with this item.
	int type() const override { return Type; }

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

	QJsonObject to_JSON() const {
		QJsonObject json;

		// TODO: add font size and face
		//QFont font() const

		QPointF pos = QGraphicsTextItem::pos();
		json["pos_x"] = pos.x();
		json["pos_y"] = pos.y();
		json["text"] = QGraphicsTextItem::toPlainText();
		return json;
	}
}; // class text


class size : public item_base, public QGraphicsItem {
	qint8 qtyofpointSet ; //quantity of set points 1,2 or 3
	qreal x1, y1; //first point  
	qreal x2, y2; //second point
	qreal L; // user space length		

public:
	enum { Type = types::e_size };

	int type() const override { return Type; }

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

	QJsonObject to_JSON() const {
		QJsonObject json;
		qDebug() << __FUNCTION__ << ": not implemented yet.";
		return json;
	}
};

} // namespace vd::items
