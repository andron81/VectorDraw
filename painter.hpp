#pragma once

//#include "VectorDraw_pch.hpp"

enum class tool_e { none, edit, line_solid, line_dashed, text, size };

class painter {
	QGraphicsView *	m_view;
	QGraphicsItem *	m_item	= nullptr; // Currently painting item
	tool_e			m_tool	= tool_e::none;
public:
	painter( QGraphicsView * p_view ) : m_view( p_view ) { Q_ASSERT( m_view ); }

	void set_tool( tool_e tool ) {
		m_tool = tool;
		if ( m_item ) {
			qDebug() << __FUNCTION__ << "set_tool(): m_item != nullptr";
			m_view->scene()->removeItem( m_item );
			m_item = nullptr;
		}
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
				} else {
					// Second mouse pressing
					// Leave the item in scene as it is
					m_item = nullptr;
				}
				break;
		}
	}

	void mouse_move_event( QMouseEvent * p_event ) {
		
		if ( m_item ) {
			switch ( m_tool ) {
				case tool_e::line_solid: [[fallthrough]];
				case tool_e::line_dashed: {
					QGraphicsLineItem * p = static_cast<QGraphicsLineItem *>( m_item );
					float xFirstPoint = p->line().x1();
					float yFirstPoint = p->line().y1();

					QPointF	secondPoint	= m_view->mapToScene( p_event->pos() );

					if ((abs(xFirstPoint-secondPoint.x())) < abs(yFirstPoint - secondPoint.y()))
					secondPoint.setX(xFirstPoint); 
					else 
					secondPoint.setY(yFirstPoint);


					p->setLine( QLineF( p->line().p1(), secondPoint ) );
				} break;
			}
		}
	}
}; // class painter
