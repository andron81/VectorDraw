
#include "VectorDraw.hpp"
#include "configuration.hpp"
#include "cfg_data_type.hpp"
#include "vd_view.hpp"


vd::view::view( QGraphicsScene * p_scene, QWidget * p_parent )
		: QGraphicsView( p_scene, p_parent ),main_win(p_parent)
		, m_painter( this )
	{


	
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
		setTransformationAnchor( QGraphicsView::AnchorUnderMouse );
		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		m_canvas = new view_canvas;
		scene()->addItem( m_canvas );

		connect( scene(), &QGraphicsScene::focusItemChanged, this,
			[&]( QGraphicsItem * p_new, QGraphicsItem * p_old, Qt::FocusReason reason ) {
					
				if (get_m_painter().get_tool_e()==tool_e::edit) {
				update_zoom();
				}
			} );

		update_zoom();
	};