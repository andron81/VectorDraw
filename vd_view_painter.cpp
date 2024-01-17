#include "configuration.hpp"
#include "vd_items.hpp"
#include "vd_view_painter.hpp"



extern cf_::Configuration * cfg;

	void vd::painter::clear_canvas(){
		
		 QList<QGraphicsItem *> itemList = m_view->items();
		 for(auto it = itemList.begin(); it != itemList.end()-1; ++it){			 
			m_view->scene()->removeItem( *it );
			delete (*it);
		 }
		
	}
	
	void vd::painter::fromJson(QJsonDocument doc){		
			clear_canvas();
			QJsonObject obj = doc.object();			
			QJsonArray items = obj["texts"].toArray();
			for (auto v : items) {
				QJsonObject element = v.toObject();
					switch (element["type"].toInt())	 {
						case -510: {
					vd::items::text* itm = new vd::items::text(QPointF(element["pos_x"].toInt(),element["pos_y"].toInt()));
					itm->setPlainText( element["text"].toString() );
					m_view->scene()->addItem(itm);					
						break;
						}
						case 6: {
					vd::items::myline* itm = new vd::items::myline(
					element["x1"].toInt(),
					element["y1"].toInt(),
					element["x2"].toInt(),
					element["y2"].toInt(), element["style"].toInt()
					, element["width"].toInt()
					);
					m_view->scene()->addItem(itm);
						break;
						}

						case -500: {
					vd::items::size* itm = new vd::items::size(element["x1"].toInt(), element["y1"].toInt(),
					element["x2"].toInt(), element["y2"].toInt(),
					element["x3"].toInt(), element["y3"].toInt(),
					QLine(element["main_line_x1"].toInt(),element["main_line_y1"].toInt(),
					element["main_line_x2"].toInt(),element["main_line_y2"].toInt()
					), QPen(),
					m_view);
					m_view->scene()->addItem(itm);
						break;
					}
							
					}
			}	
			
	}	

void vd::painter::edit_tools_on() {	
						
	cfg->getSavedValue().layout->edit_block_visible(true,&findItem) ;	
	};
void vd::painter::testy(){}

	void vd::painter::mouse_move_event( QMouseEvent * p_event ) {
		qreal mouseX = m_view->mapToScene( p_event->pos() ).x();
		qreal mouseY = m_view->mapToScene( p_event->pos() ).y();
		
		QPointF canvas_point1 = cfg->getSavedValue().layout->get_view()->get_canvas()->get_top_left() ;
		
		QSize canvas_sz = cfg->getSavedValue().layout->get_view()->get_canvas()->get_size() ;
		QPointF canvas_point2 = QPointF(canvas_sz.width(),canvas_sz.height())+canvas_point1;
		
		if (mouseX >=canvas_point1.x() && mouseX <=canvas_point2.x() && mouseY >=canvas_point1.y() && mouseY <=canvas_point2.y())
		cfg->getSavedValue().m_window->set_status_bar_text("("+QString::number(mouseX)+","+QString::number(mouseY)+")");
		else 
		{
		if ( cursor )  {delete cursor; cursor=nullptr;}
		if ( sz_itm ) {
			
			m_view->scene()->removeItem(sz_itm); delete(sz_itm); sz_itm=nullptr;
			
		}
		cfg->getSavedValue().m_window->set_status_bar_text("");		
		}
		if (!first) {
			first=true;
			lastmouseCoord.setX(mouseX);
			lastmouseCoord.setY(mouseY);
		}
		switch ( m_tool ) {
			case tool_e::size:
			if (sz_itm) { 
			if (mouseX >=canvas_point1.x() && mouseX <=canvas_point2.x() && mouseY >=canvas_point1.y() && mouseY <=canvas_point2.y())
					sz_itm->set_first_point_visible(true);
			 else sz_itm->set_first_point_visible(false);
			}
				if (!sz_itm || sz_itm->getMode()==4) {
					  if (mouseX >=canvas_point1.x() && mouseX <=canvas_point2.x() && mouseY >=canvas_point1.y() && mouseY <=canvas_point2.y()) {
									sz_itm = new items::size(mouseX,mouseY,m_view);
									m_view->scene()->addItem(sz_itm);
					  }
					  
						
				  //} 
				  //else {delete sz_itm;sz_itm=nullptr;}
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
					if (sz_itm->getMode()==3 ) {						
						sz_itm->setX3(mouseX);sz_itm->setY3(mouseY); 
						if (mouseX >=canvas_point2.x()-50) sz_itm->setX3(canvas_point2.x()-50);
						if (mouseX <=canvas_point1.x()+50) sz_itm->setX3(canvas_point1.x()+50);
						if (mouseY >=canvas_point2.y()-50) sz_itm->setY3(canvas_point2.y()-50);
						if (mouseY <=canvas_point1.y()+50) sz_itm->setY3(canvas_point1.y()+50);
						}	
					
					if (sz_itm->getMode()<4) {sz_itm->update(); }
						
				} break;
			case tool_e::line_solid: case tool_e::line_dashed:
					if ( m_item ) {
					vd::items::myline * p = static_cast<vd::items::myline *>( m_item );
					QPointF	secondPoint	= m_view->mapToScene( p_event->pos() );
					qreal xFirstPoint = p->line().x1();
					qreal yFirstPoint = p->line().y1();
					qreal xSecondPoint = secondPoint.x();
					qreal ySecondPoint = secondPoint.y();
					QPointF Coord = GetNearXYOBJECT(xFirstPoint, yFirstPoint,xSecondPoint,ySecondPoint);
					if (Coord.x() >=canvas_point2.x()) Coord.setX(canvas_point2.x());
					if (Coord.y() >=canvas_point2.y()) Coord.setY(canvas_point2.y());
					if (Coord.x() <=canvas_point1.x()) Coord.setX(canvas_point1.x());
					if (Coord.y() <=canvas_point1.y()) Coord.setY(canvas_point1.y());
					
					//if (Coord.x() <=canvas_point1.x()) Coord.setX(canvas_point1.x());
					//if (Coord.x() >=canvas_point1.x()) Coord.setX(canvas_point1.x());
						secondPoint.setX(Coord.x());secondPoint.setY(Coord.y());
					//if (secondPoint.x() >=canvas_point1.x()) && secondPoint.x() <=canvas_point2.x() && secondPoint.y() >=canvas_point1.y() && secondPoint.y() <=canvas_point2.y())						
						p->setLine( QLineF( p->line().p1(), secondPoint ) );

						
				} else {
					QPointF Coord = GetNearXYLine(mouseX, mouseY);
					if (mouseX >=canvas_point1.x() && mouseX <=canvas_point2.x() && mouseY >=canvas_point1.y() && mouseY <=canvas_point2.y())
						{	
						if (!cursor)
							
							cursor = m_view->scene()->addEllipse(Coord.x(),Coord.y(),5,5);
						
						else 
							
							cursor->setRect(Coord.x(),Coord.y(),5,5);
						}
						else if (cursor) {delete cursor; cursor=nullptr;}
						
				}	break;	
			case tool_e::edit:	{ //drag & drop item
				if (p_event->buttons().testFlag(Qt::LeftButton) && (findItem.item) ) {
					drag_and_drop=true;
					 
					if (findItem.item->type()==6){
						vd::items::myline * findLine = static_cast<vd::items::myline *>(findItem.item);	
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
					} else
					if (findItem.item->type()==-510){
						vd::items::text * txt = static_cast<vd::items::text *>(findItem.item);
						findItem.item->setX(mouseX)	;
						findItem.item->setY(mouseY)	;
					}	else			
					if (findItem.item->type()==-500){
						
						vd::items::size * findSize = static_cast<vd::items::size *>(findItem.item);	
						QLineF main_line = findSize->get_main_line();
						vd::items::size::triangle_type  coord = findSize->get_coord();
			
						if (coord.x1!=coord.x2 && coord.y1!=coord.y2) {
	 
														
								if (main_line.x1()==main_line.x2()){ //vert

									qreal deltaY1=abs(findItem.point.y()-main_line.y1());									
									qreal deltaY2=abs(findItem.point.y()-main_line.y2());	
									qreal deltaX1=abs(coord.x1-main_line.x1());	
									qreal deltaX2=abs(coord.x2-main_line.x1());									
									if (coord.x3>=coord.x1) {																		
										findSize->setX3(mouseX)	;
										findSize->setY1(mouseY-deltaY1)	;
										findSize->setY2(mouseY+deltaY2)	;
										findSize->setX1(mouseX-deltaX1)	;
										findSize->setX2(mouseX-deltaX2)	;
										findSize->setY3(mouseY+deltaY2)	;
										findItem.point.setX(mouseX);
										findItem.point.setY(mouseY);
								} else
								{	
										findSize->setX1(mouseX+deltaX1)	;
										findSize->setY1(mouseY-deltaY1)	;
										findSize->setX2(mouseX+deltaX2)	;
										findSize->setY2(mouseY+deltaY2)	;
										findSize->setX3(mouseX)	;
										findSize->setY3(mouseY+deltaY2)	;
										findItem.point.setX(mouseX);
										findItem.point.setY(mouseY);	
								}
									
									
								}										//vert end
								else
								if (main_line.y1()==main_line.y2()){ //hori

									qreal deltaX1=abs(findItem.point.x()-main_line.x1());									
									qreal deltaX2=abs(findItem.point.x()-main_line.x2());	
									qreal deltaY1=abs(coord.y1-main_line.y1());										
									qreal deltaY2=abs(coord.y2-main_line.y1());				
									if (coord.y3>=coord.y1) {										
									findSize->setY3(mouseY)	;
									findSize->setX1(mouseX-deltaX1)	;
									findSize->setX2(mouseX+deltaX2)	;
									findSize->setY1(mouseY-deltaY1)	;
									findSize->setY2(mouseY-deltaY2)	;
									findSize->setX3(mouseX+deltaX2)	;
									findItem.point.setY(mouseY);
									findItem.point.setX(mouseX);																		
								} else
									{
										
									findSize->setY3(mouseY)	;
									findSize->setX1(mouseX-deltaX1)	;
									findSize->setX2(mouseX+deltaX2)	;
									findSize->setY1(mouseY+deltaY1)	;
									findSize->setY2(mouseY+deltaY2)	;
									findSize->setX3(mouseX+deltaX2)	;
									findItem.point.setY(mouseY);
									findItem.point.setX(mouseX);
										
									}
								}								
						


						
						} else

						if (main_line.x1()==main_line.x2())  {//vertsize
							qreal deltaY1=abs(findItem.point.y()-main_line.y1());									
							qreal deltaY2=abs(findItem.point.y()-main_line.y2());	
							qreal deltaX1=abs(coord.x1-main_line.x1());		
							if (coord.x3>=coord.x1) {														
								findSize->setX3(mouseX)	;
								findSize->setY1(mouseY-deltaY1)	;
								findSize->setY2(mouseY+deltaY2)	;
								findSize->setX1(mouseX-deltaX1)	;
								findSize->setX2(mouseX-deltaX1)	;
								findSize->setY3(mouseY+deltaY2)	;
								findItem.point.setX(mouseX);
								findItem.point.setY(mouseY);
							} else
							{								
								findSize->setX1(mouseX+deltaX1)	;
								findSize->setY1(mouseY-deltaY1)	;
								findSize->setX2(mouseX+deltaX1)	;
								findSize->setY2(mouseY+deltaY2)	;
								findSize->setX3(mouseX)	;
								findSize->setY3(mouseY+deltaY2)	;
								findItem.point.setX(mouseX);
								findItem.point.setY(mouseY);	
							}
						


						
						} else
						if (main_line.y1()==main_line.y2())  {//horisize
							qreal deltaX1=abs(findItem.point.x()-main_line.x1());									
							qreal deltaX2=abs(findItem.point.x()-main_line.x2());	
							qreal deltaY1=abs(coord.y1-main_line.y1());		
							if (coord.y3>=coord.y1) {														
								findSize->setY3(mouseY)	;
								findSize->setX1(mouseX-deltaX1)	;
								findSize->setX2(mouseX+deltaX2)	;
								findSize->setY1(mouseY-deltaY1)	;
								findSize->setY2(mouseY-deltaY1)	;
								findSize->setX3(mouseX+deltaX2)	;
								findItem.point.setY(mouseY);
								findItem.point.setX(mouseX);
							} else
							{								
								findSize->setY1(mouseY+deltaY1)	;
								findSize->setX1(mouseX-deltaX1)	;
								findSize->setY2(mouseY+deltaY1)	;
								findSize->setX2(mouseX+deltaX2)	;
								findSize->setY3(mouseY)	;
								findSize->setX3(mouseX+deltaX2)	;
								findItem.point.setY(mouseY);
								findItem.point.setX(mouseX);	
							}
						


						
						}
					findSize->update();
					}
									
					
				}

			}				

		}


		


		lastmouseCoord.setX(mouseX);  lastmouseCoord.setY(mouseY);
	}


	void vd::painter::mouse_press_event( QMouseEvent * p_event ) {
		
	auto restore_old_style_elements=[](point_and_QGraphicsItem& newfindItem, point_and_QGraphicsItem& findItem){
				if (newfindItem.item!=findItem.item && findItem.item && findItem.item->type()==6) static_cast<vd::items::myline *>(findItem.item)->setPen(cfg->getSavedValue().m_line_pen);
					else
				if (findItem.item && newfindItem.item!=findItem.item && findItem.item->type()==-500 ) static_cast<vd::items::size *>(findItem.item)->setBlackColor();
					else
				if (findItem.item && newfindItem.item!=findItem.item && findItem.item->type()==-510 ) 
				{	
				static_cast<vd::items::text *>(findItem.item)->setDefaultTextColor(cfg->getSavedValue().default_color) ;
				
				}
						
	};

		
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
				if (sz_itm){
					switch 	(sz_itm->getMode()) {
						case 0:  sz_itm->setX2(mouseX);sz_itm->setY2(mouseY); sz_itm->setMode(1); 
						break;
						case  1: 
						
						sz_itm->setX3(mouseX);sz_itm->setY3(mouseY); 
						sz_itm->setMode(3); 
						sz_itm->setGreenColor();
						break;
						case  3: 
						sz_itm->setMode(4); 
						sz_itm->setBlackColor();
						m_item = nullptr;
						
						break;
						
						

					}
						if (sz_itm->getMode()<3) {sz_itm->update(); }
					
				}
				
				break;
			case tool_e::edit:	{
/********************************/	
			
			point_and_QGraphicsItem newfindItem = GetNearXYOBJECT(mouseX, mouseY);			
			if (newfindItem.item) {				
			  if (newfindItem.item->type()==-510) {	
				restore_old_style_elements(newfindItem, findItem);
				static_cast<vd::items::text *>(newfindItem.item)->setDefaultTextColor(cfg->getSavedValue().selected_color) ;				
				findItem = newfindItem;	
				edit_tools_on();
			  } else
			  if (newfindItem.item->type()==6) {

				restore_old_style_elements(newfindItem, findItem);
				//save new pen style of current line:
				if ((static_cast<vd::items::myline *>(newfindItem.item))->pen()!=
				cfg->getSavedValue().m_focused_line_pen)
				cfg->getSavedValue().m_line_pen =(static_cast<vd::items::myline *>(newfindItem.item))->pen();
				//set "focused_line_pen for a newfindItem" 		
				static_cast<vd::items::myline *>(newfindItem.item)->setPen(cfg->getSavedValue().m_focused_line_pen);
				findItem = newfindItem;				
				edit_tools_on();
				findItem.item->setFocus();
				} else
				if (newfindItem.item->type()==-500) {
					
				restore_old_style_elements(newfindItem, findItem);
				//set "focused size for a newfindItem" 		
				static_cast<vd::items::size *>(newfindItem.item)->setGreenColor();
				findItem = newfindItem;				
				edit_tools_on();
				findItem.item->setFocus();				
				}
				
				} else {				
				if (findItem.item && findItem.item->type()==6) {
				static_cast<vd::items::myline *>(findItem.item)->setPen(cfg->getSavedValue().m_line_pen);
				findItem.item=nullptr;
				cfg->getSavedValue().layout->edit_block_visible(false);
				} else 						
				if (findItem.item && findItem.item->type()==-500) {
				static_cast<vd::items::size *>(findItem.item)->setBlackColor();	;
				findItem.item=nullptr;
				cfg->getSavedValue().layout->edit_block_visible(false);
				}
				else 						
				if (findItem.item && findItem.item->type()==-510) {				
				static_cast<vd::items::text *>(findItem.item)->setDefaultTextColor(cfg->getSavedValue().default_color); 
				findItem.item=nullptr;
				
				}	
	
			}

				
				
			break;		
			}		

		}
	}
