#include "VectorDraw.hpp"
#include "configuration.hpp"


extern cf_::Configuration * cfg;

vd::view::view( QGraphicsScene * p_scene, QWidget * p_parent )
		: QGraphicsView( p_scene, p_parent ),main_win(p_parent)
		, m_painter( this )
	{

	//this->setAttribute(Qt::WA_Hover, true);
	
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		m_canvas = new view_canvas;
		scene()->addItem( m_canvas );
		
/*	connect( scene(), &QGraphicsScene::focusItemChanged, cfg->getSavedValue()->get_m_edit_width_item(),
		&vd::my_QLineEdit::test);*/
		
		connect( scene(), &QGraphicsScene::focusItemChanged, this,
			[&]( QGraphicsItem * p_new, QGraphicsItem * p_old, Qt::FocusReason reason ) {
					
				if (get_m_painter().get_tool_e()==tool_e::edit) {/*
				// Looks ugly, should be improved in future...s
				// Restore unfocused line pen...
				if (p_new && p_old && p_old->type() == QGraphicsLineItem::Type ) {					
				qDebug() << "focus items changed. p_new && p_old exists" ;
					QGraphicsLineItem * p_line = qgraphicsitem_cast<QGraphicsLineItem *>( p_old );
					Q_ASSERT( p_line && m_focus_line_pen_prev != Qt::NoPen );					
					p_line->setPen( m_focus_line_pen_prev );
				}
					else
				// Set focused line pen for contrast...
				if ( p_new && p_new->type() == QGraphicsLineItem::Type ) {
				qDebug() << "focus items changed. p_new is exists only" ;
					QGraphicsLineItem * p_line = qgraphicsitem_cast<QGraphicsLineItem *>( p_new );
					Q_ASSERT( p_line );
					m_focus_line_pen_prev = p_line->pen();
					p_line->setPen( cfg->getSavedValue().m_focus_line_pen );
				}	else
				if (p_old && p_old->type() == QGraphicsLineItem::Type ) {	
					qDebug() << "focus items changed. p_old is exists only" ;
					QGraphicsLineItem * p_line = qgraphicsitem_cast<QGraphicsLineItem *>( p_old  );
					p_line->setPen( m_focus_line_pen_prev );	
				}

				m_focus = p_new;*/
				update_zoom();
				}
			} );

		update_zoom();
	};