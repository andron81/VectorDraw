#pragma once

namespace vd::items {

enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };
enum loc { up = 0, down = 1, right = 2, left = 3, none = 4 };
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
	qint8 mode  ; // 
	qreal x1, y1; //#1  point  
	qreal x2, y2; //#2 point
	qreal x3, y3; //#3
	
	QGraphicsView *m_view;
public:
	enum { Type = types::e_size };

	int type() const override { return Type; }

	size (qreal _x1, qreal _y1, QGraphicsView * _m_view): x1(_x1), y1(_y1),mode(0) ,m_view(_m_view) {}//qtyofpointSet = 1 immediately
    	QRectF boundingRect() const override{        
	        return QRectF(200, 200,800,800);
	}
	void setMode(qint8 s) {
		mode = s;
	}
	qint8 getMode() {
		return mode;
	}
	void setX2(qreal _x2) {qDebug() << "setx2";x2=_x2;}
	void setY2(qreal _y2) {qDebug() << "sety2";y2=_y2;}
	void setX1(qreal _x1) {qDebug() << "setx1";x1=_x1;}
	void setY1(qreal _y1) {qDebug() << "sety1";y1=_y1;}
	void setX3(qreal _x3) {qDebug() << "setx3";x3=_x3;}
	void setY3(qreal _y3) {qDebug() << "sety3";y3=_y3;}
	



	
	QPointF GetNearXYOBJECT(qreal xnow, qreal ynow) {
				QList<QGraphicsItem *> itemList = m_view->items();
				int sz=itemList.size();	
				bool loca; //location of line (vert or hori)
				
				QLineF line;
				bool locaLine;
				qint8 minDistance=100;

		for (qsizetype i = 0; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);		
			//qDebug() <<"ll";
			if (item->type()==6) {	// is item line ?
			QGraphicsLineItem * tmpLine = static_cast<QGraphicsLineItem *>( item );	
				QLineF linecoord = tmpLine->line();
					if (linecoord.x1()==linecoord.x2()) loca = true; else loca = false;
					//qDebug() << "mouse :"<<xnow << " "<<ynow<< " line#"<<i<<linecoord.x1()<<" "<<linecoord.y1() ;
					if (loca && minDistance>=abs(xnow-linecoord.x1()) && ((xnow+10>=linecoord.x1() && xnow<=linecoord.x1()) || (xnow-10<=linecoord.x1() && xnow>=linecoord.x1())) ) {
						minDistance = abs(xnow-linecoord.x1()); 
						line=linecoord; 
						locaLine=loca;
						} 
					if (!loca && abs(ynow-linecoord.y1()) && ((ynow+10>=linecoord.y1() && ynow<=linecoord.y1()) || (ynow-10<=linecoord.y1() && ynow>=linecoord.y1())) )  {						
						minDistance = abs(ynow-linecoord.y1()); 
						line=linecoord; locaLine=loca; }
				
			}
				
	}
		QPointF res;
		
	if (minDistance==100) res=QPointF(xnow,ynow); else
	if (locaLine) res= QPointF(line.x1(),ynow ); else res= QPointF(xnow,line.y1() );
	return res;
	}
	
	
	QRectF boundingRect() {
    return QRectF(x1,y1,x3,y3);
	}
	
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) override{
			
		switch  ( mode ) {
			case  0:
			
				painter->drawEllipse(x1,y1,5,5);
				
			break;
			case  1 : case  2 :	
			painter->drawEllipse(x1,y1,5,5);
			painter->drawEllipse(x2,y2,5,5);
			break;
			case  3 :case  4:
			//painter->drawEllipse(x1,y1,5,5);
			//painter->drawEllipse(x2,y2,5,5);		
			if (y1==y2) {
				if (abs(x1-x2)<40) {
					painter->drawLine(x1-15,y3,x2+15,y3);
				painter->drawLine(x1,y3,x1-8,y3+8);
				painter->drawLine(x1,y3,x1-8,y3-8);				
				painter->drawLine(x2,y3,x2+8,y3-8);
				painter->drawLine(x2,y3,x2+8,y3+8);
				}
				else {
					painter->drawLine(x1,y3,x2,y3);
				painter->drawLine(x1,y3,x1+8,y3+8);
				painter->drawLine(x1,y3,x1+8,y3-8);				
				painter->drawLine(x2,y3,x2-8,y3+8);
				painter->drawLine(x2,y3,x2-8,y3-8);
				}
				
				if (y3>y2)
				painter->drawText(QPoint(x1+abs(x2-x1)/2,y3	+15),QString::number(abs(x2-x1)));
				else 
				painter->drawText(QPoint(x1+abs(x2-x1)/2,y3	-15),QString::number(abs(x2-x1)));
				
				
				
				painter->drawLine(x1,y2,x1,y3);
				painter->drawLine(x2,y2,x2,y3);
				} else
			if (x1==x2) {
				
				if (abs(y1-y2)<40) {
				painter->drawLine(x3,y1-15,x3,y2+15);
				painter->drawLine(x3,y1,x3-8,y1-8);
				painter->drawLine(x3,y1,x3+8,y1-8);
				painter->drawLine(x3,y2,x3-8,y2+8);
				painter->drawLine(x3,y2,x3+8,y2+8);
				} else {
					painter->drawLine(x3,y1,x3,y2);
				painter->drawLine(x3,y1,x3+15,y1+10);
				painter->drawLine(x3,y1,x3-15,y1+10);
				painter->drawLine(x3,y2,x3+15,y2-10);
				painter->drawLine(x3,y2,x3-15,y2-10);
				}
				
				painter->drawLine(x1,y1,x3,y1);
				painter->drawLine(x2,y2,x3,y2);
				if (x3>x2)
				painter->drawText(QPoint(x3+15,y1+abs(y2-y1)/2),QString::number(abs(y2-y1)));
				else 
					painter->drawText(QPoint(x3-25,y1+abs(y2-y1)/2),QString::number(abs(y2-y1)));
				}	else
			/*if (x1!=x2 && y1!=y2) {	
				loc location;
				if x3>
				
			}*/
				
			//painter->drawEllipse(x2,y2,15,15);
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
