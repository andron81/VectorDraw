#pragma once
#include "vd_items.hpp"

const int step = 7;
const qreal penWidth=3;

namespace vd {

enum class tool_e { none, edit, line_solid, line_dashed, text, size, remove };

class view;

class painter {

public:

	struct point_and_QGraphicsItem{
	QGraphicsItem* item;
	QPointF point;
	QLineF firstCoord;	
};

private:	
	QGraphicsView  *	m_view;
	QGraphicsItem *	m_item	= nullptr; // Currently painting item
	tool_e			m_tool	= tool_e::none;
	items::size  * sz_itm = nullptr;
	QGraphicsEllipseItem *	cursor	= nullptr; //Bad to declare here
	point_and_QGraphicsItem findItem = {nullptr, QPointF(0.0f,0.0f) , QLineF(0.0f,0.0f,0.0f,0.0f) };
	bool			first;
	QPointF			lastmouseCoord;

	

public:


	
	painter(  QGraphicsView * p_view  ) :  m_view( p_view ), first(false){Q_ASSERT( m_view ); }
	
	void set_tool( tool_e tool ) {
		m_tool = tool;		
		if ( m_item ) { // If item is not drawn completely, remove it	
			qDebug() << __FUNCTION__ << "1set_tool(): m_item != nullptr";
			m_view->scene()->removeItem( m_item );
			m_item = nullptr;
		}
		if ( sz_itm ) {
			m_view->scene()->removeItem( sz_itm );
			sz_itm = nullptr;
		}		
	}
	void edit_tools_on() ;
	void testy() ;
	
	QPointF GetNearXYLine(qreal x0, qreal y0) { //Search find a line near
		QList<QGraphicsItem *> itemList = m_view->items();
		int sz=itemList.size();	
		qreal mindistance=2000;
		qreal xResult = x0,  yResult = y0;
				for (qsizetype i = 0; i < sz; i++) { 
					QGraphicsItem* item=itemList.at(i);							
					if (item->type()==6) { //if item line
						QGraphicsLineItem * tmpLine = static_cast<QGraphicsLineItem *>( item );
						QLineF linecoord = tmpLine->line();
							if (linecoord.x1()==linecoord.x2() ) {//if vert line		
								if (x0+step>=linecoord.x1() && x0<=linecoord.x1() && mindistance>abs(x0-linecoord.x1())) {mindistance=abs(x0-linecoord.x1()); xResult=linecoord.x1();yResult=y0;}
								if (x0-step<=linecoord.x1() && x0>=linecoord.x1() && mindistance>abs(x0-linecoord.x1())) {mindistance=abs(x0-linecoord.x1()); xResult=linecoord.x1();yResult=y0;}
							} //else
							if (linecoord.y1()==linecoord.y2() ) {//if hori line										
								if (y0+step>=linecoord.y1() && y0<=linecoord.y1() && mindistance>abs(y0-linecoord.y1())) {mindistance=abs(y0-linecoord.y1()); yResult=linecoord.y1();xResult=x0;}
								if (y0-step<=linecoord.y1() && y0>=linecoord.y1() && mindistance>abs(y0-linecoord.y1())) {mindistance=abs(y0-linecoord.y1()); yResult=linecoord.y1();xResult=x0;}
							} 
					}
				}
		return QPointF(xResult,yResult);				
	}		
	
	point_and_QGraphicsItem  GetNearXYOBJECT(qreal xnow, qreal ynow) {
				QList<QGraphicsItem *> itemList = m_view->items();
				int sz=itemList.size();	
				bool loca; //location of line (vert or hori)
				
				QLineF line;
				bool locaLine;
				qint8 minDistance=100;
			QGraphicsLineItem  *tmpLine;
			point_and_QGraphicsItem result{nullptr}; 
			
			
		for (qsizetype i = 0; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);		
			//qDebug() <<"ll";
			if (item->type()==6) {	// is item line ?
			 tmpLine = static_cast<QGraphicsLineItem *>( item );	
				QLineF linecoord = tmpLine->line();
				linecoord.setP1(linecoord.p1() +item->pos());
				linecoord.setP2(linecoord.p2() +item->pos());
					loca = (linecoord.x1()==linecoord.x2());
					if (loca && minDistance>=abs(xnow-linecoord.x1()) && ((xnow+10>=linecoord.x1() && xnow<=linecoord.x1()) || (xnow-10<=linecoord.x1() && xnow>=linecoord.x1())) ) {
						minDistance = abs(xnow-linecoord.x1()); 
						result.item = item;
						result.point = {xnow,ynow};
						result.firstCoord = tmpLine->line();
						locaLine=loca;
						} 
					if (!loca && abs(ynow-linecoord.y1()) && ((ynow+10>=linecoord.y1() && ynow<=linecoord.y1()) || (ynow-10<=linecoord.y1() && ynow>=linecoord.y1())) )  {						
						minDistance = abs(ynow-linecoord.y1());
						result.item = item;
						result.point = {xnow,ynow};
						result.firstCoord = tmpLine->line();
						 locaLine=loca; }
				
			}
				
	}
		QPointF res;
	qDebug()<<"exec"	;
	if (minDistance==100) return {nullptr}; else {qDebug()<<minDistance; return result;}
	
	
	}
	
	QPointF GetNearXYOBJECT(qreal x0, qreal y0, qreal x1, qreal y1) {

		bool loca; //location of line (vert or hori)
		QList<QGraphicsItem *> itemList = m_view->items();
		int sz=itemList.size();		
		if (abs(x0-x1) < abs(y0 - y1)) {x1 = x0; loca=true; }
			else {y1 = y0; loca=false; }		
		qreal xResult=x1, yResult=y1;
		for (qsizetype i = 1; i < sz; i++) { 
			QGraphicsItem* item=itemList.at(i);							
			if (item->type()==6) {							
				QGraphicsLineItem * tmpLine = static_cast<QGraphicsLineItem *>( item );				
				QLineF linecoord = tmpLine->line();
				linecoord.setP1(linecoord.p1() +item->pos());
				linecoord.setP2(linecoord.p2() +item->pos());
				qreal minX = std::min(linecoord.x1(),linecoord.x2());
				qreal minY = std::min(linecoord.y1(),linecoord.y2());
				qreal maxX = std::max(linecoord.x1(),linecoord.x2());
				qreal maxY = std::max(linecoord.y1(),linecoord.y2());
				

				if (x1+step >=linecoord.x1() && x1<=linecoord.x1() && !loca && (linecoord.x1()==linecoord.x2() ) 
					&& y1>=minY
					&& y1<=maxY  )  
					{					
					xResult = linecoord.x1(); break;
					} else
				if (x1-step <=linecoord.x1() && x1>=linecoord.x1() && !loca && (linecoord.x1()==linecoord.x2()) 
					&& y1>=minY
					&& y1<=maxY)  
					{					
					xResult = linecoord.x1(); break;
					} 
					else
				if (y1+step >=linecoord.y1() && y1<=linecoord.y1() && loca && (linecoord.y1()==linecoord.y2())  
					&& x1>=minX
					&& x1<=maxX) 
					{					
					yResult = linecoord.y1();break;
					} 
					else
				if (y1-step <=linecoord.y1() && y1>=linecoord.y1() && loca && (linecoord.y1()==linecoord.y2()) 
					&& x1>=minX
					&& x1<=maxX)  
					{
					
					yResult = linecoord.y1(); break;}
					} //else qDebug()<<"oops";
				} //for
				if (!(xResult==x0 || yResult==y0))  {xResult= x1; yResult= y1;}
				return QPointF(xResult,yResult);
		}
	
	void mouse_press_event( QMouseEvent * p_event ) {
		qreal mouseX = m_view->mapToScene( p_event->pos() ).x();
		qreal mouseY = m_view->mapToScene( p_event->pos() ).y();
		QPointF	pt	=  GetNearXYLine(mouseX, mouseY); 
		QPen	pen	= QPen( Qt::SolidLine ); // Default pen
		pen.setWidth(penWidth);
		switch ( m_tool ) {
			case tool_e::line_solid: [[fallthrough]];
			case tool_e::line_dashed:					
				if ( m_tool == tool_e::line_dashed ) {
					pen = QPen( Qt::DashLine);pen.setWidth(penWidth);
					qDebug()<<"select";
				}                                        	

				if ( !m_item ) {
					m_item = m_view->scene()->addLine( QLineF( pt, pt ), pen );
					m_item->setFlags(  QGraphicsItem::ItemIsFocusable );
					m_item->setToolTip( "Test Tooltip" );
					qDebug()<< "Test Tooltip" ;
				} else {
					// Second mouse pressing
					// Leave the item in scene as it is					
					m_item = nullptr;
				}
				break;
			case tool_e::text:
				m_view->scene()->addItem( new items::text( pt ) );
				break;
			case tool_e::size: 
			//qDebug() <<"sz click";
					
				if (sz_itm){
					switch 	(sz_itm->getMode()) {
						case 0: qDebug() <<"set fisrt point of size"; sz_itm->setX2(mouseX);sz_itm->setY2(mouseY); sz_itm->setMode(1); 
						break;
						case  1: qDebug() <<"set second point of size"; 
							sz_itm->setX3(mouseX);sz_itm->setY3(mouseY); 
						sz_itm->setMode(3); 
						sz_itm->setGreenColor();
						break;
						case  3: qDebug() << "click #2"; //sz_itm->setX2(mouseX);sz_itm->setY2(mouseY); 				
						//sz_itm->setX3(mouseX);sz_itm->setY3(mouseY); 
						sz_itm->setMode(4); 
						m_item = nullptr;
						sz_itm->setBlackColor();
						break;
						
						

					}
						if (sz_itm->getMode()<3) {qDebug() <<"update call" ; sz_itm->update(); }
					
				}
				
				break;
			case tool_e::edit:	{
			if (findItem.item) findItem.item->clearFocus();
			findItem = GetNearXYOBJECT(mouseX, mouseY);
			if (findItem.item) {
				QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(findItem.item);	
				//m_view->get_main_window();
			
			edit_tools_on();
				findItem.item->setFocus();
		
			}
				
			break;		
			}		
			
		}
	}

	void mouse_release_event( QMouseEvent * p_event ) {qDebug() << "realease---";}

	void mouse_move_event( QMouseEvent * p_event ) {
		qreal mouseX = m_view->mapToScene( p_event->pos() ).x();
		qreal mouseY = m_view->mapToScene( p_event->pos() ).y();
		
		if (!first) {
			first=true;
			lastmouseCoord.setX(mouseX);
			lastmouseCoord.setY(mouseY);
		}
		switch ( m_tool ) {
			case tool_e::size:
				if (!sz_itm || sz_itm->getMode()==4) {
				sz_itm = new items::size(mouseX,mouseY,m_view);
				m_view->scene()->addItem(sz_itm);
				} else {				
					if (sz_itm->getMode()==0) {
					
					QPointF coord=sz_itm->GetNearXYOBJECT(mouseX, mouseY);
					sz_itm->setX1(coord.x());sz_itm->setY1(coord.y()); 
					} else 
					if (sz_itm->getMode()==1) {
					
					QPointF coord=sz_itm->GetNearXYOBJECT(mouseX, mouseY);
					sz_itm->setX2(coord.x());sz_itm->setY2(coord.y()); 
					} else	
					if (sz_itm->getMode()==2) {
					
					QPointF coord=sz_itm->GetNearXYOBJECT(mouseX, mouseY);
					//sz_itm->setX2(coord.x());sz_itm->setY2(coord.y());  
					} else
					if (sz_itm->getMode()==3) {sz_itm->setX3(mouseX);sz_itm->setY3(mouseY);}	
						if (sz_itm->getMode()<4) {sz_itm->update(); }
						
				} break;
			case tool_e::line_solid: case tool_e::line_dashed:
					if ( m_item ) {
					QGraphicsLineItem * p = static_cast<QGraphicsLineItem *>( m_item );
					QPointF	secondPoint	= m_view->mapToScene( p_event->pos() );
					qreal xFirstPoint = p->line().x1();
					qreal yFirstPoint = p->line().y1();
					qreal xSecondPoint = secondPoint.x();
					qreal ySecondPoint = secondPoint.y();
					QPointF Coord = GetNearXYOBJECT(xFirstPoint, yFirstPoint,xSecondPoint,ySecondPoint);
						secondPoint.setX(Coord.x());secondPoint.setY(Coord.y());					
							p->setLine( QLineF( p->line().p1(), secondPoint ) );	
				} else {
					QPointF Coord = GetNearXYLine(mouseX, mouseY);
						if (!cursor)
							
							cursor = m_view->scene()->addEllipse(Coord.x(),Coord.y(),5,5);
						
						else 
							
							cursor->setRect(Coord.x(),Coord.y(),5,5);
						
				}	break;	
			case tool_e::edit:	{ //drag & drop item
			//findItem = GetNearXYOBJECT(mouseX, mouseY);
				if (p_event->buttons().testFlag(Qt::LeftButton) && (findItem.item) ) {

					 ;
					
					QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(findItem.item);	
					
								
								if (findLine->line().y1()==findLine->line().y2()) {//horiline
									qreal delta1=abs(findItem.point.x()-findItem.firstCoord.x1());									
									qreal delta2=abs(findItem.point.x()-findItem.firstCoord.x2());									
								  findLine->setLine(mouseX-delta1, mouseY, mouseX+delta2, mouseY);									  
								} else
								if (findLine->line().x1()==findLine->line().x2()) {//horiline
									qreal delta1=abs(findItem.point.y()-findItem.firstCoord.y1());									
									qreal delta2=abs(findItem.point.y()-findItem.firstCoord.y2());									
								  findLine->setLine(mouseX, mouseY-delta1, mouseX, mouseY+delta2);									  
								} 
									
					
				}

			}				

		}


		


		lastmouseCoord.setX(mouseX);  lastmouseCoord.setY(mouseY);
	}
}; // class painter

} // namespace vd

