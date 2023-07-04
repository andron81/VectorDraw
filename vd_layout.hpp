#pragma once

namespace vd {

// All classes that contain signals or slots must mention Q_OBJECT at the top of their declaration. They must also derive (directly or indirectly) from QObject.
class layout : public QObject {
	Q_OBJECT

	QMainWindow *		m_parent;

	QLineEdit *			m_edit_width;
	QLineEdit *			m_edit_height;

	QGraphicsScene *	m_scene;
	vd::view *			m_view;

	QSize get_size() const {
		return {
			m_edit_width ->text().toInt(),
			m_edit_height->text().toInt() };
	}

public:
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

		{ // Left side
			QVBoxLayout * p_layout_ctrl = new QVBoxLayout( p_widget_ctrl );

			auto add_tool_button = [this]( QLayout * p_layout, QWidget * p_parent, const QString & icon, const QString & name, auto && slot ) {
				QPushButton * p = new QPushButton( p_parent );
				p->setFixedHeight( 32 );
				p->setFlat( true );
				p->setCheckable( true );
				p->setIcon( QIcon( icon ) );
				p->setToolTip( name );

				// If auto-exclusivity is enabled, checkable buttons that belong to the same parent widget behave as if they were part of the same exclusive button group.
				// In an exclusive button group, only one button can be checked at any time; checking another button automatically unchecks the previously checked one.
				p->setAutoExclusive( true );

				p_layout->addWidget( p );
				connect( p, &QPushButton::toggled, m_parent, slot );
			};

			auto add_hbox = [&]( auto * p_parent, auto && hbox ) {
				QHBoxLayout * l = new QHBoxLayout;
				hbox( l );
				p_parent->addLayout( l );
			};

			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, "icons/tool_edit.svg"       , "Редактирование", [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::edit ); } } );
				add_tool_button( p, p_widget_ctrl, "icons/tool_size.svg"       , "Размер",         [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::size ); } } );
			} );

			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, "icons/tool_line_solid.svg" , "Сплошная",       [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::line_solid ); } } );
				add_tool_button( p, p_widget_ctrl, "icons/tool_line_dashed.svg", "Пунктирная",     [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::line_dashed ); } } );
			} );

			add_hbox( p_layout_ctrl, [&]( QHBoxLayout * p ) {
				add_tool_button( p, p_widget_ctrl, "icons/tool_text.svg"       , "Текст",          [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::text ); } } );
				add_tool_button( p, p_widget_ctrl, "icons/tool_remove.svg"     , "Удалить",        [&]( bool toggled ){ if ( toggled ) { m_view->set_tool( vd::tool_e::remove ); } } );
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

			p_layout_ctrl->addStretch( 0 );
		}

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

} // namespace vd
