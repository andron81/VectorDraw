#include "configuration.hpp"	
#include "vd_layout.hpp"
  
	vd::layout::layout( QMainWindow * p_parent )
		: QObject( p_parent )
		, m_parent( p_parent )
	{
		Q_ASSERT( m_parent );

		// Central widget
		QWidget * p_widget_central = new QWidget( m_parent );
		m_parent->setCentralWidget( p_widget_central );	

		QHBoxLayout * p_layout_central = new QHBoxLayout( p_widget_central );
		p_layout_central->setSpacing( 0 );
		p_layout_central->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );

		QWidget * p_widget_ctrl = new QWidget( p_widget_central ); p_widget_ctrl->setMaximumWidth( 100 );
	

		QWidget * p_widget_view = new QWidget( p_widget_central );

		p_layout_central->addWidget( p_widget_ctrl );
		p_layout_central->addWidget( p_widget_view );

		//{ // Left side
		//auto mse_current_coordinat_label = new QLabel("0:0");	
			
			//p_layout_mouse_coord->addWidget( mse_current_coordinat_label,Qt::AlignBottom );
			QVBoxLayout * p_layout_ctrl = new QVBoxLayout( p_widget_ctrl );
			
			
			
			
			auto add_tool_button = [this]( QLayout * p_layout, QWidget * p_parent, const QString & icon, const QString & name, auto && slot ) {
				QPushButton * p = new QPushButton( p_parent );
				p->setFixedHeight( 32 );
				p->setFlat( true );
				p->setCheckable(true);
				p->setIcon( QIcon( icon ) );
				p->setToolTip( name );

				// If auto-exclusivity is enabled, checkable buttons that belong to the same parent widget behave as if they were part of the same exclusive button group.
				// In an exclusive button group, only one button can be checked at any time; checking another button automatically unchecks the previously checked one.
				p->setAutoExclusive(true);

				p_layout->addWidget( p );
				connect( p, &QPushButton::toggled, m_parent, slot );
				return p;
			};

			auto add_arrow_button = [this]( QLayout * p_layout, QWidget * p_parent, const QString & icon, const QString & name, auto && slot ) {
				not_toggled_buttons.push_back(new QPushButton( p_parent ));
				auto p =not_toggled_buttons.back();
				p->setFixedHeight( 32 );				
				//p->setCheckable(true);
				p->setFlat( true);
				p->setIcon( QIcon( icon ) );
				p->setToolTip( name );
				p->setEnabled(false);
				// If auto-exclusivity is enabled, checkable buttons that belong to the same parent widget behave as if they were part of the same exclusive button group.
				// In an exclusive button group, only one button can be checked at any time; checking another button automatically unchecks the previously checked one.
				//p->setAutoExclusive(true);

				p_layout->addWidget( p );
				connect( p, &QPushButton::clicked, m_parent, slot );
				return p;
			};



			auto add_hbox = [&]( auto * p_parent, auto && hbox ) {
				QHBoxLayout * l = new QHBoxLayout;
				hbox( l );
				p_parent->addLayout( l );
			};
			
			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, ":/images/tool_edit.svg"       , "Редактирование", [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::edit ); } } );
				add_tool_button( p, p_widget_ctrl, ":/images/tool_size.svg"       , "Размер",         [&]( bool toggled ){ if ( toggled ) { m_edit_width_item->remove_finditem();m_view->set_tool( vd::tool_e::size ); } } );
			} );

			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, ":/images/tool_line_solid.svg" , "Сплошная",       [&]( bool toggled ){ if ( toggled ) { m_edit_width_item->remove_finditem();m_view->set_tool( vd::tool_e::line_solid ); } } );
				add_tool_button( p, p_widget_ctrl, ":/images/tool_line_dashed.svg", "Пунктирная",     [&]( bool toggled ){ if ( toggled ) { m_edit_width_item->remove_finditem();m_view->set_tool( vd::tool_e::line_dashed ); } } );
			} );

			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, ":/images/tool_text.svg"       , "Текст",          [&]( bool toggled ){ if ( toggled ) { m_edit_width_item->remove_finditem();m_view->set_tool( vd::tool_e::text ); } } );
				add_arrow_button( p, p_widget_ctrl, ":/images/tool_remove.svg"     , "Удалить",        [&]( bool toggled ){ 
				if (m_edit_width_item->get_find_item()->item)
				
				m_scene->removeItem( m_edit_width_item->get_find_item()->item);
				
				delete(m_edit_width_item->get_find_item()->item);
				
				m_edit_width_item->get_find_item()->item=nullptr;
				edit_block_visible(false);
				} 
				);
			} );



			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_arrow_button( p, p_widget_ctrl, ":/images/right_svgrepo_com.svg"       , "Переместить вправо",    

				[&]( bool toggled ){ 
				auto m_find_item = m_edit_width_item->get_find_item();								
				if (m_find_item && m_find_item->item) {
					switch (m_find_item->item->type()) {
					case 6: {	
					auto m_focus = qgraphicsitem_cast<vd::items::myline *>(m_find_item->item);
					QLineF lineTmp=m_focus->line();
					lineTmp.setP1(QPointF(lineTmp.p1().x()+1,lineTmp.p1().y()));			
					lineTmp.setP2(QPointF(lineTmp.p2().x()+1,lineTmp.p2().y()));			
					m_focus->setLine(lineTmp);
					m_edit_width_item->set_finditem(m_find_item);
					break;
					}
					case -500: {					
					vd::items::size * m_focus = static_cast<vd::items::size *>(m_find_item->item);	
					vd::items::size::triangle_type coord = m_focus->get_coord();					
					m_focus->setX1(coord.x1+1);
					m_focus->setX2(coord.x2+1);
					m_focus->setX3(coord.x3+1);					
					m_focus->update();										
					break;
					}
					case -510: {					
					m_find_item->item->setPos((m_find_item->item->pos().x())+1,m_find_item->item->pos().y());	
					break;
					}
				} 
				
				}
				}

				);
				add_arrow_button( p, p_widget_ctrl, ":/images/left_svgrepo_com.svg"     , "Переместить влево",        





				[&]( bool toggled ){ 
				auto m_find_item = m_edit_width_item->get_find_item();								
				if (m_find_item && m_find_item->item) {
					switch (m_find_item->item->type()) {
					case 6: {	
					auto m_focus = qgraphicsitem_cast<vd::items::myline *>(m_find_item->item);
					QLineF lineTmp=m_focus->line();
					lineTmp.setP1(QPointF(lineTmp.p1().x()-1,lineTmp.p1().y()));			
					lineTmp.setP2(QPointF(lineTmp.p2().x()-1,lineTmp.p2().y()));			
					m_focus->setLine(lineTmp);
					m_edit_width_item->set_finditem(m_find_item);
					break;
					}
					case -500: {					
					vd::items::size * m_focus = static_cast<vd::items::size *>(m_find_item->item);	
					vd::items::size::triangle_type coord = m_focus->get_coord();					
					m_focus->setX1(coord.x1-1);
					m_focus->setX2(coord.x2-1);
					m_focus->setX3(coord.x3-1);					
					m_focus->update();										
					break;
					}
					case -510: {					
					m_find_item->item->setPos((m_find_item->item->pos().x())-1,m_find_item->item->pos().y());	
					break;
					}
				} 
				
				}
				}



				
				
				);
			} );



			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_arrow_button( p, p_widget_ctrl, ":/images/up_svgrepo_com.svg"       , "Переместить вверх",          
				

				[&]( bool toggled ){ 
				auto m_find_item = m_edit_width_item->get_find_item();								
				if (m_find_item && m_find_item->item) {
					switch (m_find_item->item->type()) {
					case 6: {	
					auto m_focus = qgraphicsitem_cast<vd::items::myline *>(m_find_item->item);
					QLineF lineTmp=m_focus->line();
					lineTmp.setP1(QPointF(lineTmp.p1().x(),lineTmp.p1().y()-1));			
					lineTmp.setP2(QPointF(lineTmp.p2().x(),lineTmp.p2().y()-1));			
					m_focus->setLine(lineTmp);
					m_edit_width_item->set_finditem(m_find_item);
					break;
					}
					case -500: {					
					vd::items::size * m_focus = static_cast<vd::items::size *>(m_find_item->item);	
					vd::items::size::triangle_type coord = m_focus->get_coord();					
					m_focus->setY1(coord.y1-1);
					m_focus->setY2(coord.y2-1);
					m_focus->setY3(coord.y3-1);					
					m_focus->update();										
					break;
					}
					case -510: {					
					m_find_item->item->setPos((m_find_item->item->pos().x()),m_find_item->item->pos().y()-1);	
					break;
					}
				} 
				
				}
				}

				
				
				);
				add_arrow_button( p, p_widget_ctrl, ":/images/down_svgrepo_com.svg"     , "Переместить вниз",        


				[&]( bool toggled ){ 
				auto m_find_item = m_edit_width_item->get_find_item();								
				if (m_find_item && m_find_item->item) {
					switch (m_find_item->item->type()) {
					case 6: {	
					auto m_focus = qgraphicsitem_cast<vd::items::myline *>(m_find_item->item);
					QLineF lineTmp=m_focus->line();
					lineTmp.setP1(QPointF(lineTmp.p1().x(),lineTmp.p1().y()+1));			
					lineTmp.setP2(QPointF(lineTmp.p2().x(),lineTmp.p2().y()+1));			
					m_focus->setLine(lineTmp);
					m_edit_width_item->set_finditem(m_find_item);
					break;
					}
					case -500: {					
					vd::items::size * m_focus = static_cast<vd::items::size *>(m_find_item->item);	
					vd::items::size::triangle_type coord = m_focus->get_coord();					
					m_focus->setY1(coord.y1+1);
					m_focus->setY2(coord.y2+1);
					m_focus->setY3(coord.y3+1);					
					m_focus->update();										
					break;
					}
					case -510: {					
					m_find_item->item->setPos((m_find_item->item->pos().x()),m_find_item->item->pos().y()+1);	
					break;
					}
				} 
				
				}
				}


				);
			} );


			

			p_layout_ctrl->addWidget( new QLabel( "Размер (мм.):" ) );
			{
				QString hs = cf_::Configuration::Instance()->getSavedValue().hs;
				QString h = hs.left(hs.indexOf('='));
				QString w = hs.right(hs.indexOf('='));
				if (h=="") h="700";
				if (w=="") w="1000";
				QHBoxLayout * layout_ctrl_size = new QHBoxLayout;

				m_edit_width = new QLineEdit( w, m_parent );
				m_edit_width->setValidator( new QIntValidator( 10/*min*/, 1999/*max*/, m_parent ) );
				layout_ctrl_size->addWidget( m_edit_width );

				layout_ctrl_size->addWidget( new QLabel( "x", m_parent ) );

				m_edit_height = new QLineEdit( h, m_parent );
				m_edit_height->setValidator( new QIntValidator( 10/*min*/, 1999/*max*/, m_parent ) );
				layout_ctrl_size->addWidget( m_edit_height );
							
				
				p_layout_ctrl->addLayout( layout_ctrl_size );

				connect(  m_edit_width, &QLineEdit::editingFinished, this, size_changed );
				connect( m_edit_height, &QLineEdit::editingFinished, this, size_changed );
			}
			size_element_label = new QLabel("Размер \n элемента");
			
			p_layout_ctrl->addWidget( size_element_label );
		
			m_edit_width_item = new size_edit("100",m_parent  );	

			p_layout_ctrl->addWidget( m_edit_width_item  );
			edit_block_visible(false,nullptr);

		
			
				
			p_layout_ctrl->addStretch( 0 );

		
		{ // Right side
			m_scene = new QGraphicsScene( -1000/*x*/, -1000/*y*/, 2000/*w*/, 2000/*h*/, p_widget_view );
			m_view = new vd::view( m_scene, p_widget_view );
			m_view->set_canvas_size( get_size() );
			p_layout_central->addWidget( m_view );
		}
		m_view->setFocus();
		
	}