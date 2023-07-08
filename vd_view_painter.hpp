#pragma once
const int step=30;
namespace vd {

enum class tool_e { none, edit, line_solid, line_dashed, text, size, remove };

class painter {
	QGraphicsView *	m_view;
	QGraphicsItem *	m_item	= nullptr; // Currently painting item
	tool_e			m_tool	= tool_e::none;
	bool first;
	QPointF lastmouseCoord;

public:
	painter( QGraphicsView * p_view ) : m_view( p_view ), first(false) { Q_ASSERT( m_view ); }

	void set_tool( tool_e tool ) {
		m_tool = tool;
		if ( m_item ) {
			qDebug() << __FUNCTION__ << "set_tool(): m_item != nullptr";
			m_view->scene()->removeItem( m_item );
			m_item = nullptr;
		}
	}

std::optional<QPointF> GetNearXYOBJECT(double x0, double y0, double x1, double y1){
		enum loc {up =0 ,down =1,right=2 ,left =3} ;
		loc location;
	QList<QGraphicsItem *> itemList = m_view->items();
						if (x1<lastmouseCoord.x()) location = left; 
				   else if (x1>lastmouseCoord.x()) location = right; 
				   else if (y1<lastmouseCoord.y()) location = up; 
				   else if (y1>lastmouseCoord.y()) location = down;
							if (abs(x0-x1) < abs(y0 - y1))
							x1 = x0; 
							else 
							y1 = y0; 
							double xResult=x1, yResult=y1;

int sz=itemList.size();
		for (qsizetype i = 1; i < sz; i++) { 
							QGraphicsItem* item=itemList.at(i);
							qDebug() <<"line(" << i;
			if (item->type()==6) {
				
				QGraphicsLineItem * tmpLine = static_cast<QGraphicsLineItem *>( item );
				//qDebug() <<"line(" << i<< ") x1=" <<i<<tmpLine->line().x1()<< " y1="<<tmpLine->line().y1()<<" x2="<<tmpLine->line().x2()<< " "<<" y2="<<tmpLine->line().y2()<<":" << x1<<","<<y1<<" "<<location	;
				if (x1+step >=tmpLine->line().x1() && x1<tmpLine->line().x1() && location == right)  {qDebug() <<"r*"; xResult = tmpLine->line().x1(); yResult = y1; break;} else 
				if (x1-step <=tmpLine->line().x1() && x1>tmpLine->line().x1() &&location == left)  { qDebug() <<"l*"; xResult = tmpLine->line().x1(); yResult = y1; break;} else
				if (y1+step >=tmpLine->line().y1() && y1<tmpLine->line().y1() && location == down)  { qDebug() <<"u*"; yResult = tmpLine->line().y1(); xResult = x1; break;} else
				if (y1-step <=tmpLine->line().y1() && y1>tmpLine->line().y1() && location == up)  { qDebug() <<"u*"; yResult = tmpLine->line().y1(); xResult = x1; break;} 
			}

			
}
			return {QPointF(xResult,yResult)};
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
		}
	}


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
					float xFirstPoint = p->line().x1();
					float yFirstPoint = p->line().y1();
					float xSecondPoint = secondPoint.x();
					float ySecondPoint = secondPoint.y();
					//qDebug() << "("<<lastmouseCoord.x()<<","<<lastmouseCoord.y()<<") - "<< "("<<xSecondPoint<<","<<ySecondPoint<<")"; 
					//std::optional<QPointF> Coord=
					std::optional<QPointF> Coord = GetNearXYOBJECT(xFirstPoint, yFirstPoint,xSecondPoint,ySecondPoint);
					if (Coord) {						
						secondPoint.setX(Coord->x());secondPoint.setY(Coord->y());
						}
					p->setLine( QLineF( p->line().p1(), secondPoint ) );
				} break;
			}
		}
	lastmouseCoord.setX(m_view->mapToScene( p_event->pos() ).x());  lastmouseCoord.setY(m_view->mapToScene( p_event->pos() ).y());
	}
}; // class painter

} // namespace vd
