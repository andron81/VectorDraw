#include "vd_items.hpp"
#include "vd_view_painter.hpp"

vd::items::item_base::item_base(vd::painter * m_painter_ ):m_painter(m_painter_) {}

QPen vd::items::item_base::get_pen(qint8 style , qint8 width){
	QPen pen;
		switch (style) {
			case 1: pen.setStyle(Qt::SolidLine);break;
			case 2: pen.setStyle(Qt::DashLine);break;
			case 3: pen.setStyle(Qt::DotLine);break;
			case 4: pen.setStyle(Qt::DashDotLine);break;						
		}
		pen.setWidth(width);
		return pen;
};

vd::items::myline::myline(vd::painter * painter_ , qreal x1, qreal y1, qreal x2, qreal y2, int style,int width ):
	item_base(painter_),
	QGraphicsLineItem(x1, y1,x2, y2){
	setPen(get_pen(style , width));	
};

QJsonObject vd::items::myline::to_JSON() const {
	QJsonObject json;
	QLineF line=this->line();
			json["type"] = type();
			json["style"] = static_cast<int>(pen().style());
			json["width"] = pen().width();
			json["x1"]=line.x1();
			json["y1"]=line.y1();
			json["x2"]=line.x2();
			json["y2"]=line.y2();
		return json;
};



vd::items::text::text(vd::painter * painter_ , const QPointF & pos ):
		item_base(painter_), 
		is_select(false){
			setPos( pos );
			setFont( QFont( "Times", 72) );
			setPlainText( "Текст" );
	}

void vd::items::text::set_is_select(bool value) {is_select = value;}
bool vd::items::text::get_is_select() {return is_select;}
void vd::items::text::change_size_by( int points ) {
	QFont f = font();
	f.setPointSize( f.pointSize() + points );
	setFont( f );
}
void vd::items::text::click(){}
QJsonObject vd::items::text::to_JSON() const {
	QJsonObject json;
		QPointF pos = QGraphicsTextItem::pos();
		json["type"] = type();
		json["pos_x"] = pos.x();
		json["pos_y"] = pos.y();
		json["text"] = QGraphicsTextItem::toPlainText();
		return json;
}

void vd::items::text::mousePressEvent( QGraphicsSceneMouseEvent  * p_event ) {
	vd::tool_e tool = m_painter->get_tool_e();
	if (tool == vd::tool_e::edit)
	is_select=true;
}

vd::items::size::triangle_type  vd::items::size::get_coord(){ return {x1,y1,x2,y2,x3,y3	}; }	
void vd::items::size::set_first_point_visible(bool value) {first_point_visible = value;}	
vd::items::size::size(vd::painter * painter_ , qreal _x1, qreal _y1): 
	x1(_x1), 
	y1(_y1),
	mode(0) ,
	item_base(painter_),
	pen( Qt::SolidLine ),
	first_point_visible(true) {}
	
vd::items::size::size (vd::painter * painter_ , qreal _x1, qreal _y1,
		  qreal _x2, qreal _y2,
		  qreal _x3, qreal _y3,
		  QLineF _main_line, QPen _pen
		  ): item_base(painter_),
		  x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3),main_line(_main_line), mode(4) ,
		pen( _pen),first_point_visible(false) {}
	vd::items::size::~size(){}	
		
    QRectF vd::items::size::boundingRect() const{        
	        return QRectF(200, 200,800,800);
	}
	
	void vd::items::size::setMode(qint8 s) {
		mode = s;
	}
	void vd::items::size::setBlackColor() {		
		pen.setColor(Qt::black);
		update();
	}		
	void vd::items::size::setGreenColor() {
		pen.setColor(Qt::green);
		update();
	}	
	bool vd::items::size::get_first_point_visible(){return first_point_visible;}	
	
	qint8 vd::items::size::getMode() {
		return mode;
	}
	void vd::items::size::setX2(qreal _x2) {x2=_x2;}
	void vd::items::size::setY2(qreal _y2) {y2=_y2;}
	void vd::items::size::setX1(qreal _x1) {x1=_x1;}
	void vd::items::size::setY1(qreal _y1) {y1=_y1;}
	void vd::items::size::setX3(qreal _x3) {x3=_x3;}
	void vd::items::size::setY3(qreal _y3) {y3=_y3;}
	

	QLineF vd::items::size::get_main_line() {return main_line;}

	
	QPointF vd::items::size::GetNearXYOBJECT(qreal xnow, qreal ynow) {		
				QList<QGraphicsItem *> itemList = m_painter->get_m_view()->items();
				int sz=itemList.size();	
				bool loca; //location of line (vert or hori)
				
				QLineF line;
				bool locaLine;
				qint8 minDistance=100;

		for (qsizetype i = 0; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);		
			if (item->type()==6) {	// is item line ?
			QGraphicsLineItem * tmpLine = static_cast<QGraphicsLineItem *>( item );	
				QLineF linecoord = tmpLine->line();
					loca = (linecoord.x1()==linecoord.x2());
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
	
	
	
	void vd::items::size::swapPoint(){
		qreal tmp;
		tmp=x1; x1=x2; x2=tmp;
		tmp=y1; y1=y2; y2=tmp;
	}
	void vd::items::size::sortPoint() {
	if ((y1==y2 and x1>x2) || (x1==x2 and y1>y2)) 
		swapPoint();				
	}
	void vd::items::size::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget){
	
		//pen.setWidth(1);
		
		painter->setPen(pen);
		
		switch  ( mode ) {
			case  0:	
				
				if (first_point_visible) painter->drawEllipse(x1,y1,10,10);
				
			break;
			case  1 : case  2 :	
			painter->drawEllipse(x1,y1,5,5);
			painter->drawEllipse(x2,y2,5,5);
			break;
			case  3 :case  4:
			sortPoint();
			//painter->drawEllipse(x1,y1,5,5);
			//painter->drawEllipse(x2,y2,5,5);		
			if (y1==y2) {
				if (abs(x1-x2)<40) {
				painter->drawLine(x1-15,y3,x2+15,y3); 
				main_line=QLineF(x1,y3,x2,y3);//set mainline 
				painter->drawLine(x1,y3,x1-8,y3+8);
				painter->drawLine(x1,y3,x1-8,y3-8);				
				painter->drawLine(x2,y3,x2+8,y3-8);
				painter->drawLine(x2,y3,x2+8,y3+8);
				}
				else {
					painter->drawLine(x1,y3,x2,y3); //!
					main_line=QLineF(x1,y3,x2,y3);//set mainline 
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
				painter->drawLine(x3,y1-15,x3,y2+15); //!
				main_line=QLineF(x3,y1,x3,y2);//set mainline 
				painter->drawLine(x3,y1,x3-8,y1-8);
				painter->drawLine(x3,y1,x3+8,y1-8);
				painter->drawLine(x3,y2,x3-8,y2+8);
				painter->drawLine(x3,y2,x3+8,y2+8);
				} else {
					painter->drawLine(x3,y1,x3,y2);   //!
					main_line=QLineF(x3,y1,x3,y2);//set mainline  
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
				}	else {
					qreal tmp;
					//if (x1>x2) {tmp=x1; x1=x2; x2=tmp;}
					//if (y1>y2) {tmp=y1; y1=y2; y2=tmp;}
							
						if (x3>=x1 && x3<=x2 && y3>=y1 && y3<=y2) {
						
						 if (abs(x2-x1)>40){							 
							painter->drawLine(x1,y1,x1,y3+15);
							painter->drawLine(x2,y2,x2,y3-15);
							painter->drawLine(x1,y3,x2,y3);
							main_line=QLineF(x1,y3,x2,y3);//set mainline 
							painter->drawLine(x1,y3,x1+8,y3-8);
							painter->drawLine(x1,y3,x1+8,y3+8);							
							painter->drawLine(x2,y3,x2-8,y3-8);
							painter->drawLine(x2,y3,x2-8,y3+8);
							painter->drawText(QPoint(x1+abs(x2-x1)/2,y3	+15),QString::number(abs(x2-x1)));
							}
							else{	
							painter->drawLine(x1,y1,x1,y3+15);
							painter->drawLine(x2,y2,x2,y3-15);
							painter->drawLine(x1-15,y3,x2+15,y3);
							main_line=QLineF(x1,y3,x2,y3);//set mainline 
							painter->drawLine(x1,y3,x1-8,y3+8);
							painter->drawLine(x1,y3,x1-8,y3-8);							
							painter->drawLine(x2,y3,x2+8,y3+8);
							painter->drawLine(x2,y3,x2+8,y3-8);
							painter->drawText(QPoint(x1+abs(x2-x1)/2,y3	+15),QString::number(abs(x2-x1)));								
							}
						}
						else
						if (x3>=x1 && x3>=x2 && y3>=y1 && y3<=y2) {
							if (abs(x2-x1)>40){
							painter->drawLine(x1,y1,x3,y1);
							painter->drawLine(x2,y2,x3,y2);
							painter->drawLine(x3,y1,x3,y2);
							main_line=QLineF(x3,y1,x3,y2);//set mainline 
							painter->drawLine(x3,y1,x3-8,y1+8);
							painter->drawLine(x3,y1,x3+8,y1+8);
							
							painter->drawLine(x3,y2,x3-8,y2-8);
							painter->drawLine(x3,y2,x3+8,y2-8);
							painter->drawText(QPoint(x3+5,y1+abs(y2-y1)/2),QString::number(abs(y2-y1)));
							}
							else {
							painter->drawLine(x1,y1,x3,y1);
							painter->drawLine(x2,y2,x3,y2);
							painter->drawLine(x3,y1-15,x3,y2+15);
							main_line=QLineF(x3,y1,x3,y2);//set mainline 
							painter->drawLine(x3,y1,x3-8,y1-8);
							painter->drawLine(x3,y1,x3+8,y1-8);
							
							painter->drawLine(x3,y2,x3-8,y2+8);
							painter->drawLine(x3,y2,x3+8,y2+8);
							painter->drawText(QPoint(x3,y1+abs(y2-y1)/2),QString::number(abs(y2-y1)));
							}
							
							}
					else
							if (x3<x1 && x3<x2 && y3>=y1 && y3<=y2) {
							if (abs(x2-x1)>40){
							painter->drawLine(x1,y1,x3,y1);
							painter->drawLine(x2,y2,x3,y2);
							painter->drawLine(x3,y1,x3,y2);
							main_line=QLineF(x3,y1,x3,y2);
							painter->drawLine(x3,y1,x3-8,y1+8);
							painter->drawLine(x3,y1,x3+8,y1+8);
							
							painter->drawLine(x3,y2,x3-8,y2-8);
							painter->drawLine(x3,y2,x3+8,y2-8);
							painter->drawText(QPoint(x3+5,y1+abs(y2-y1)/2),QString::number(abs(y2-y1)));
							}
							else {
							painter->drawLine(x1,y1,x3,y1);
							painter->drawLine(x2,y2,x3,y2);
							painter->drawLine(x3,y1-15,x3,y2+15);
							main_line=QLineF(x3,y1,x3,y2);
							painter->drawLine(x3,y1,x3-8,y1-8);
							painter->drawLine(x3,y1,x3+8,y1-8);
							
							painter->drawLine(x3,y2,x3-8,y2+8);
							painter->drawLine(x3,y2,x3+8,y2+8);
							painter->drawText(QPoint(x3,y1+abs(y2-y1)/2),QString::number(abs(y2-y1)));
							}
							
							}
					else
							if ((y3>y1 && y3>y2) ) {
							if (abs(x2-x1)>40){
							painter->drawLine(x1,y1,x1,y3+8);
							painter->drawLine(x2,y2,x2,y3+8);
							
							painter->drawLine(x1,y3,x2,y3);
							main_line=QLineF(x1,y3,x2,y3);
							painter->drawLine(x1,y3,x1+8,y3+8);
							painter->drawLine(x1,y3,x1+8,y3-8);
							
							painter->drawLine(x2,y3,x2-8,y3+8);
							painter->drawLine(x2,y3,x2-8,y3-8);
					
							painter->drawText(QPoint(x1+abs(x2-x1)/2,y3+13),QString::number(abs(x2-x1)));
							}
							else {

							painter->drawLine(x1,y1,x1,y3+8);
							painter->drawLine(x2,y2,x2,y3+8);
							
							painter->drawLine(x1-15,y3,x2+15,y3);
							main_line=QLineF(x1,y3,x2,y3);
							painter->drawLine(x1,y3,x1-8,y3+8);
							painter->drawLine(x1,y3,x1-8,y3-8);
							
							painter->drawLine(x2,y3,x2+8,y3+8);
							painter->drawLine(x2,y3,x2+8,y3-8);
					
							painter->drawText(QPoint(x1+abs(x2-x1)/2,y3+13),QString::number(abs(x2-x1)));


							}
							
							}
							else
							if ((y3<y1 && y3<y2) ) {
							if (abs(x2-x1)>40){
							painter->drawLine(x1,y1,x1,y3-8);
							painter->drawLine(x2,y2,x2,y3-8);
							
							painter->drawLine(x1,y3,x2,y3);
							main_line=QLineF(x1,y3,x2,y3);
							painter->drawLine(x1,y3,x1+8,y3+8);
							painter->drawLine(x1,y3,x1+8,y3-8);
							
							painter->drawLine(x2,y3,x2-8,y3+8);
							painter->drawLine(x2,y3,x2-8,y3-8);
							painter->drawText(QPoint(x1+abs(x2-x1)/2,y3-8),QString::number(abs(x2-x1)));
							}
							else {
							painter->drawLine(x1,y1,x1,y3-8);
							painter->drawLine(x2,y2,x2,y3-8);

							painter->drawLine(x1-15,y3,x2+15,y3);
							main_line=QLineF(x1,y3,x2,y3);
							painter->drawLine(x1,y3,x1-8,y3+8);
							painter->drawLine(x1,y3,x1-8,y3-8);
							
							painter->drawLine(x2,y3,x2+8,y3+8);
							painter->drawLine(x2,y3,x2+8,y3-8);

							painter->drawText(QPoint(x1+abs(x2-x1)/2,y3-8),QString::number(abs(x2-x1)));
							}
							
							}							
							
							
		
				
				}

			break;
			
		}
		
    }

	QJsonObject vd::items::size::to_JSON() const {

		QJsonObject json;		
		json["type"] = type();
		json["style"] = static_cast<int>(pen.style());
		json["width"] = pen.width();
		json["x1"]=x1;
		json["y1"]=y1;
		json["x2"]=x2;
		json["y2"]=y2;
		json["x3"]=x3;
		json["y3"]=y3;
		
		json["main_line_x1"]=main_line.x1();
		json["main_line_y1"]=main_line.x1();
		json["main_line_x2"]=main_line.x2();
		json["main_line_y2"]=main_line.y2();
		

		
		return json;

	}