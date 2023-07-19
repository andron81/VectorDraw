﻿#pragma once

const int step = 7;

namespace vd {

enum class tool_e { none, edit, line_solid, line_dashed, text, size, remove };
enum loc { up = 0, down = 1, right = 2, left = 3, none = 4 };

class painter {
	QGraphicsView *	m_view;
	QGraphicsItem *	m_item	= nullptr; // Currently painting item
	tool_e			m_tool	= tool_e::none;
	items::size  * sz_itm = nullptr;
	bool			first;
	QPointF			lastmouseCoord;

public:
	painter( QGraphicsView * p_view ) : m_view( p_view ), first(false){ Q_ASSERT( m_view ); }
	void set_tool( tool_e tool ) {
		m_tool = tool;
		if ( m_item ) { // If item is not drawn completely, remove it
			qDebug() << __FUNCTION__ << "set_tool(): m_item != nullptr";
			m_view->scene()->removeItem( m_item );
			m_item = nullptr;
		}
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
				if (x1+step >=tmpLine->line().x1() && x1<=tmpLine->line().x1() && !loca && (tmpLine->line().x1()==tmpLine->line().x2() ) 
					&& y1>=std::min(tmpLine->line().y1(),tmpLine->line().y2())
					&& y1<=std::max(tmpLine->line().y1(),tmpLine->line().y2())  )  
					{					
					xResult = tmpLine->line().x1(); break;
					} else
				if (x1-step <=tmpLine->line().x1() && x1>=tmpLine->line().x1() && !loca && (tmpLine->line().x1()==tmpLine->line().x2() ) 
					&& y1>=std::min(tmpLine->line().y1(),tmpLine->line().y2()) 
					&& y1<=std::max(tmpLine->line().y1(),tmpLine->line().y2()))  
					{					
					xResult = tmpLine->line().x1(); break;
					} 
					else
				if (y1+step >=tmpLine->line().y1() && y1<=tmpLine->line().y1() && loca && (tmpLine->line().y1()==tmpLine->line().y2())  
					&& x1>=std::min(tmpLine->line().x1(),tmpLine->line().x2())
					&& x1<=std::max(tmpLine->line().x1(),tmpLine->line().x2()) 
					)  
					{					
					yResult = tmpLine->line().y1();break;
					} 
					else
				if (y1-step <=tmpLine->line().y1() && y1>=tmpLine->line().y1() && loca && (tmpLine->line().y1()==tmpLine->line().y2()) && 
					   x1>=std::min(tmpLine->line().x1(),tmpLine->line().x2())
					&& x1<=std::max(tmpLine->line().x1(),tmpLine->line().x2()))  
					{
					
					yResult = tmpLine->line().y1(); break;}
					}
				} //for
				if (!(xResult==x0 || yResult==y0))  {xResult= x1; yResult= y1;}
				return QPointF(xResult,yResult);
		}

	void mouse_press_event( QMouseEvent * p_event ) {

		QPointF	pt	= m_view->mapToScene( p_event->pos() );
		QPen	pen	= QPen( Qt::SolidLine ); // Default pen
		switch ( m_tool ) {
			case tool_e::line_solid: [[fallthrough]];
			case tool_e::line_dashed:					
				if ( m_tool == tool_e::line_dashed ) {
					pen = QPen( Qt::DashLine );
				}                                        	

				if ( !m_item ) {
					// First mouse pressing
					m_item = m_view->scene()->addLine( QLineF( pt, pt ), pen );
					m_item->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
					m_item->setToolTip( "Test Tooltip" );

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
					qDebug() << "size!";
				if (!sz_itm){// First mouse clicked for create tool "Size"				
					sz_itm = new items::size(pt.x(),pt.y());
					m_view->scene()->addItem(sz_itm);
				}	
				else {		//Second mouse clicked
					sz_itm->setX2(pt.x());
					sz_itm->setY2(pt.y());	
					sz_itm->setqtyofPoint(2);
					sz_itm->update();
				}	
				
				break;
		}
	}

	void mouse_release_event( QMouseEvent * p_event ) {}

	void mouse_move_event( QMouseEvent * p_event ) {

		if (!first) {
			first=true;
			lastmouseCoord.setX(m_view->mapToScene( p_event->pos() ).x());
			lastmouseCoord.setY(m_view->mapToScene( p_event->pos() ).y());
		}

		if ( m_item ) {
			switch ( m_tool ) {
				case tool_e::line_solid: [[fallthrough]];
				case tool_e::line_dashed: {
					QGraphicsLineItem * p = static_cast<QGraphicsLineItem *>( m_item );
					QPointF	secondPoint	= m_view->mapToScene( p_event->pos() );
					qreal xFirstPoint = p->line().x1();
					qreal yFirstPoint = p->line().y1();
					qreal xSecondPoint = secondPoint.x();
					qreal ySecondPoint = secondPoint.y();
					QPointF Coord = GetNearXYOBJECT(xFirstPoint, yFirstPoint,xSecondPoint,ySecondPoint);
						secondPoint.setX(Coord.x());secondPoint.setY(Coord.y());					
							p->setLine( QLineF( p->line().p1(), secondPoint ) );
				} break;
			}
		}

		lastmouseCoord.setX(m_view->mapToScene( p_event->pos() ).x());  lastmouseCoord.setY(m_view->mapToScene( p_event->pos() ).y());
	}
}; // class painter

} // namespace vd
