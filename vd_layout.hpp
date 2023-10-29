#pragma once
#include "vd_size_edit.hpp"
#include "vd_view.hpp"


	
namespace vd {



// All classes that contain signals or slots must mention Q_OBJECT at the top of their declaration. They must also derive (directly or indirectly) from QObject.
class layout : public QObject {
	Q_OBJECT

	QMainWindow *		m_parent;
	QLineEdit *			m_edit_width;
	QLineEdit *			m_edit_height;
	vd::size_edit *	 	m_edit_width_item;

	QGraphicsScene *	m_scene;
	vd::view *			m_view;
	QLabel* size_element_label;
	QSize get_size() const {
		return {
			m_edit_width ->text().toInt(),
			m_edit_height->text().toInt() };
	}

public:
	void edit_block_visible(bool visible, vd::painter::point_and_QGraphicsItem* find_item_=nullptr){
		m_edit_width_item->set_finditem(find_item_);
		if (find_item_)	{QGraphicsLineItem * findLine = static_cast<QGraphicsLineItem *>(find_item_->item);	
		if (abs(findLine->line().x1()-findLine->line().x2())>0)
		m_edit_width_item->setText(QString::number(abs(findLine->line().x1()-findLine->line().x2())));
		else
		m_edit_width_item->setText(QString::number(abs(findLine->line().y1()-findLine->line().y2())));
		
		}
			size_element_label->setVisible(visible);
			m_edit_width_item->setVisible(visible);
	}
	vd::size_edit  *	 get_m_edit_width_item() {return m_edit_width_item;}
	
	layout( QMainWindow * p_parent )
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
			QVBoxLayout * p_layout_ctrl = new QVBoxLayout( p_widget_ctrl );
			QVBoxLayout * p_layout_ctrl_arrow_grp = new QVBoxLayout( p_widget_ctrl );
			
			
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
				QPushButton * p = new QPushButton( p_parent );
				p->setFixedHeight( 32 );				
				//p->setCheckable(true);
				p->setFlat( true);
				p->setIcon( QIcon( icon ) );
				p->setToolTip( name );

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
				add_tool_button( p, p_widget_ctrl, ":/images/tool_remove.svg"     , "Удалить",        [&]( bool toggled ){ if ( toggled ) { m_edit_width_item->remove_finditem();m_view->set_tool( vd::tool_e::remove ); } } );
			} );



			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_arrow_button( p, p_widget_ctrl, ":/images/right_svgrepo_com.svg"       , "Переместить вправо",    
				[&]( bool toggled ){ //if ( toggled ) { //m_view->set_tool( vd::tool_e::move_right );
				auto m_find_item = m_edit_width_item->get_find_item();
				
				
				qDebug()<<m_find_item;
				if (m_find_item) {
					auto m_focus = qgraphicsitem_cast<QGraphicsLineItem *>(m_find_item->item);
					 qDebug()<<"exsist"; 	

				QLineF lineTmp=m_focus->line();
				lineTmp.setP1(QPointF(lineTmp.p1().x()+10,lineTmp.p1().y()));			
				lineTmp.setP2(QPointF(lineTmp.p2().x()+10,lineTmp.p2().y()));			
				m_focus->setLine(lineTmp);


					m_edit_width_item->set_finditem(m_find_item);
					//m_focus->setFocus();	
				} else {qDebug()<<"empty";}
				//}
				} 
				);
				add_tool_button( p, p_widget_ctrl, ":/images/left_svgrepo_com.svg"     , "Переместить влево",        [&]( bool toggled ){ if ( toggled ) { qDebug()<<"lft"; m_view->set_tool( vd::tool_e::move_left ); } } );
			} );



			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, ":/images/up_svgrepo_com.svg"       , "Переместить вверх",          [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::move_up ); } } );
				add_tool_button( p, p_widget_ctrl, ":/images/down_svgrepo_com.svg"     , "Переместить вниз",        [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::move_down ); } } );
			} );


			

			p_layout_ctrl->addWidget( new QLabel( "Размер (мм.):" ) );
			{
				QHBoxLayout * layout_ctrl_size = new QHBoxLayout;

				m_edit_width = new QLineEdit( "1000", m_parent );
				m_edit_width->setValidator( new QIntValidator( 10/*min*/, 1999/*max*/, m_parent ) );
				layout_ctrl_size->addWidget( m_edit_width );

				layout_ctrl_size->addWidget( new QLabel( "x", m_parent ) );

				m_edit_height = new QLineEdit( "700", m_parent );
				m_edit_height->setValidator( new QIntValidator( 10/*min*/, 1999/*max*/, m_parent ) );
				layout_ctrl_size->addWidget( m_edit_height );
							
				
				p_layout_ctrl->addLayout( layout_ctrl_size );

				connect(  m_edit_width, &QLineEdit::editingFinished, this, size_changed );
				connect( m_edit_height, &QLineEdit::editingFinished, this, size_changed );
			}
			size_element_label = new QLabel("Размер \n элемента");
			
			p_layout_ctrl->addWidget( size_element_label );
		
			m_edit_width_item = new size_edit("100",m_parent  );	
			//m_edit_width_item->setReadOnly(true);
			p_layout_ctrl->addWidget( m_edit_width_item  );
			edit_block_visible(false,nullptr);


				
			p_layout_ctrl->addStretch( 0 );
		//}

		{ // Right side
			m_scene = new QGraphicsScene( -1000/*x*/, -1000/*y*/, 2000/*w*/, 2000/*h*/, p_widget_view );
			//m_scene->setFocusOnTouch( true );
			m_view = new vd::view( m_scene, p_widget_view );
			m_view->set_canvas_size( get_size() );
			p_layout_central->addWidget( m_view );
		}
		m_view->setFocus();
	}

	const QGraphicsScene * get_scene() const { return m_scene; }
	QGraphicsScene *       get_scene()       { return m_scene; }

	const vd::view *       get_view()  const { return m_view;  }
	vd::view *             get_view()        { return m_view;  }

private slots:
	void size_changed() {
		m_view->set_canvas_size( get_size() );
	}
}; // class layout

}; // namespace vd
