#pragma once
#include "configuration.hpp"
#include "cfg_data_type.hpp"
#include "vd_layout.hpp"



enum class vd::tool_e ;

namespace vd::items {

enum types { e_line_solid = QGraphicsItem::UserType + 1, e_line_dashed, e_text, e_size };
enum loc { up = 0, down = 1, right = 2, left = 3, none = 4 };
struct item_base {
	virtual int type() const = 0;
	virtual QJsonObject to_JSON() const = 0;
	QPen get_pen(qint8 style , qint8 width){
		QPen pen;
		switch (style) {
			case 1: pen.setStyle(Qt::SolidLine);break;
			case 2: pen.setStyle(Qt::DashLine);break;
			case 3: pen.setStyle(Qt::DotLine);break;
			case 4: pen.setStyle(Qt::DashDotLine);break;						
		}
		pen.setWidth(width);
		return pen;
	}
}; // class item

class myline final: public item_base,public QGraphicsLineItem {
	public:
enum { Type = 6 };
myline(qreal x1, qreal y1, qreal x2, qreal y2, int style,int width ):QGraphicsLineItem(x1, y1,x2, y2){
	setPen(get_pen(style , width));
	
}
int type() const override { return Type; }
//line(QGraphicsItem *parent = nullptr): QGraphicsLineItem(parent){}
QJsonObject to_JSON() const {
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
	}
}	;

class text final : public item_base, public QGraphicsTextItem {
	bool is_select;
public:
	enum { Type = -510 };

	// Enable the use of qgraphicsitem_cast with this item.
	int type() const override { return Type; }

	text( const QPointF & pos ): is_select(false) {
		setPos( pos );
		setFont( QFont( "Times", 72/*, QFont::Bold*/ ) );
		
		//setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
		//p_text->setToolTip( "Test text tooltip" );
		setPlainText( "Текст" );
		//to_JSON();
		
	}
	void set_is_select(bool value) {is_select = value;}
	bool get_is_select() {return is_select;}
	void change_size_by( int points ) {
		QFont f = font();
		f.setPointSize( f.pointSize() + points );
		setFont( f );
	}
	void click(){}
	QJsonObject to_JSON() const {
		QJsonObject json;
		QPointF pos = QGraphicsTextItem::pos();
		json["type"] = type();
		json["pos_x"] = pos.x();
		json["pos_y"] = pos.y();
		json["text"] = QGraphicsTextItem::toPlainText();
 		return json;
	}
	
	
		void mousePressEvent( QGraphicsSceneMouseEvent  * p_event ) {
		vd::tool_e tool = cf_::Configuration<cfg_data_type>::Instance().getData().layout->get_view()->get_m_painter().get_tool_e();
		if (tool == vd::tool_e::edit)
		is_select=true;		
		}
	
	
}; // class text


class size : public item_base, public QGraphicsItem {
	qint8 mode  ; // 
	QLineF main_line; // for select tool 

	qreal x1, y1; //#1  point  
	qreal x2, y2; //#2 
	qreal x3, y3; //#3
	bool first_point_visible;
	QGraphicsView *m_view;
	QPen	pen;
public:

	struct triangle_type {
	qreal x1, y1; //#1  point  
	qreal x2, y2; //#2 point
	qreal x3, y3; //#3
	};

	enum { Type = types::e_size };

	triangle_type  get_coord(){ return {x1,y1,x2,y2,x3,y3	}; }
	int type() const override { return -500; }
	void set_first_point_visible(bool value) {first_point_visible = value;}	
	size (qreal _x1, qreal _y1, QGraphicsView * _m_view): x1(_x1), y1(_y1),mode(0) ,m_view(_m_view), 
		pen( Qt::SolidLine ),first_point_visible(true) {}

	size (qreal _x1, qreal _y1,
		  qreal _x2, qreal _y2,
		  qreal _x3, qreal _y3,
		  QLineF _main_line, QPen _pen,
		  QGraphicsView * _m_view): x1(_x1), y1(_y1), x2(_x2), y2(_y2), x3(_x3), y3(_y3),main_line(_main_line), mode(4) ,m_view(_m_view), 
		pen( _pen),first_point_visible(false) {}
	~size(){}	
		
    	QRectF boundingRect() const override{        
	        return QRectF(200, 200,800,800);
	}
	
	void setMode(qint8 s) {
		mode = s;
	}
	void setBlackColor() {		
		pen.setColor(Qt::black);
		update();
	}		
	void setGreenColor() {
		pen.setColor(Qt::green);
		update();
	}	
	bool get_first_point_visible(){return first_point_visible;}	
	
	qint8 getMode() {
		return mode;
	}
	void setX2(qreal _x2) {x2=_x2;}
	void setY2(qreal _y2) {y2=_y2;}
	void setX1(qreal _x1) {x1=_x1;}
	void setY1(qreal _y1) {y1=_y1;}
	void setX3(qreal _x3) {x3=_x3;}
	void setY3(qreal _y3) {y3=_y3;}
	

	QLineF get_main_line() {return main_line;}

	
	QPointF GetNearXYOBJECT(qreal xnow, qreal ynow) {
				QList<QGraphicsItem *> itemList = m_view->items();
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
	
	
	QRectF boundingRect() {
    return QRectF(x1,y1,x3,y3);
	}
	void swapPoint(){
		qreal tmp;
		tmp=x1; x1=x2; x2=tmp;
		tmp=y1; y1=y2; y2=tmp;
	}
	void sortPoint() {
	if ((y1==y2 and x1>x2) || (x1==x2 and y1>y2)) 
		swapPoint();				
	}
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget) override{
	
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

	QJsonObject to_JSON() const {

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
};

}; // namespace vd::items
