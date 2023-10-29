#include "vd_items.hpp"
#include "vd_view_painter.hpp"
#include "configuration.hpp"


extern cf_::Configuration * cfg;
void vd::painter::edit_tools_on() {	
						
	cfg->getSavedValue().layout->edit_block_visible(true,&findItem) ;	
	};
void vd::painter::testy(){}

	void vd::painter::mouse_move_event( QMouseEvent * p_event ) {
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
			//try to find a new line for edit:
			/*
			if (!drag_and_drop) {
			point_and_QGraphicsItem findItemNew=GetNearXYOBJECT(mouseX, mouseY); 
			
			
			if (findItemNew.item) findItem=findItemNew; 			
			
			}*/
				if (p_event->buttons().testFlag(Qt::LeftButton) && (findItem.item) ) {
					drag_and_drop=true;
					 qDebug()<<"findItem.item is exists";
					
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


	void vd::painter::mouse_press_event( QMouseEvent * p_event ) {
		qreal mouseX = m_view->mapToScene( p_event->pos() ).x();
		qreal mouseY = m_view->mapToScene( p_event->pos() ).y();
		QPointF	pt	=  GetNearXYLine(mouseX, mouseY); 
		QPen	pen	= QPen( Qt::SolidLine ); // Default pen
		auto penWidth = cfg->getSavedValue().pen_width;
		pen.setWidth(penWidth);
		switch ( m_tool ) {
			case tool_e::line_solid: [[fallthrough]];
			case tool_e::line_dashed:					
				if ( m_tool == tool_e::line_dashed ) {
					pen = QPen( Qt::DashLine);pen.setWidth(penWidth);					
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
						case  3: qDebug() << "click #2"; 
						sz_itm->setMode(4); 
						m_item = nullptr;
						sz_itm->setBlackColor();
						break;
						
						

					}
						if (sz_itm->getMode()<3) {qDebug() <<"update call" ; sz_itm->update(); }
					
				}
				
				break;
			case tool_e::edit:	{
				
			point_and_QGraphicsItem newfindItem = GetNearXYOBJECT(mouseX, mouseY);
			if (newfindItem.item) {				
				qDebug()<<"del findItem.item";
			
				//restore pen style for old line:			
				if (newfindItem.item!=findItem.item && findItem.item ) static_cast<QGraphicsLineItem *>(findItem.item)->setPen(cfg->getSavedValue().m_line_pen);
				//save new pen style of current line:
				if ((static_cast<QGraphicsLineItem *>(newfindItem.item))->pen()!=
				cfg->getSavedValue().m_focused_line_pen)
				cfg->getSavedValue().m_line_pen =(static_cast<QGraphicsLineItem *>(newfindItem.item))->pen();
				//set "focused_line_pen for a newfindItem" 		
				static_cast<QGraphicsLineItem *>(newfindItem.item)->setPen(cfg->getSavedValue().m_focused_line_pen);





				
				
				
				findItem = newfindItem;				
				edit_tools_on();
				findItem.item->setFocus();				
				
			} else {				
				if (findItem.item ) {static_cast<QGraphicsLineItem *>(findItem.item)->setPen(cfg->getSavedValue().m_line_pen);
				findItem.item=nullptr;
				cfg->getSavedValue().layout->edit_block_visible(false);
				}
				
			}
				
			break;		
			}		

		}
	}
