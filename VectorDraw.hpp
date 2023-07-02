#pragma once

#include "VectorDraw_pch.hpp"
#include "settings.hpp"
#include "view.hpp"
#include "painter.hpp"	// tool_e
#include "util.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT

	settings			m_settings;

	QLineEdit *			m_edit_width;
	QLineEdit *			m_edit_height;

	QGraphicsScene *	m_scene;
	graphics_view *		m_view;

	QSize get_size() const {
		return {
			m_edit_width ->text().toInt(),
			m_edit_height->text().toInt() };
	}

	void create_menu_bar() {
		auto add_menu_action = [this]( QMenu * p_menu, const QString & name, auto && slot ) {
			QAction * p_act = new QAction( name, this );
			connect( p_act, &QAction::triggered, this, slot );
			p_menu->addAction( p_act );
		};

		// File
		QMenu * p_file = menuBar()->addMenu( "&Файл" );
		add_menu_action( p_file, "&Новый"			, &MainWindow::act_new );
		add_menu_action( p_file, "&Сохранить PNG"	, &MainWindow::act_save_png );
		add_menu_action( p_file, "&Печать"			, &MainWindow::act_print );
		p_file->addSeparator();
		add_menu_action( p_file, "Вы&ход"			, &MainWindow::close );

		// Help
		QMenu * p_help = menuBar()->addMenu( "&Помощь" );

		add_menu_action( p_help, "О программе", [this]{
				QMessageBox::about( this,
					"О VectorDraw",
					"<p><b>VectorDraw</b> версия 0.1<br></p>"
					"<p><a href='https://github.com/andron81/VectorDraw'>VectorDraw на GitHub</a></p>" );
			} );
		add_menu_action( p_help, "О &Qt", &QApplication::aboutQt );
	}

	void create_controls() {

		// Central widget
		QWidget * p_widget_central = new QWidget( this );
		setCentralWidget( p_widget_central );


		QHBoxLayout * p_central_layout = new QHBoxLayout( p_widget_central );
		p_central_layout->setSpacing( 0 );
		p_central_layout->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );

		QWidget * p_widget_ctrl = new QWidget( p_widget_central ); p_widget_ctrl->setMaximumWidth( 140 );
		QWidget * p_widget_view = new QWidget( p_widget_central );

		p_central_layout->addWidget( p_widget_ctrl );
		p_central_layout->addWidget( p_widget_view );

		{ // Left side
			QVBoxLayout * p_layout_ctrl = new QVBoxLayout( p_widget_ctrl );
			p_layout_ctrl->setObjectName( "p_layout_ctrl" ); // For DEBUG

			auto add_tool_button = [this]( QLayout * p_layout, QWidget * p_parent, const QString & name, auto && slot ) {
				QPushButton * p = new QPushButton( name, p_parent );
				p->setCheckable( true );

				// If auto-exclusivity is enabled, checkable buttons that belong to the same parent widget behave as if they were part of the same exclusive button group.
				// In an exclusive button group, only one button can be checked at any time; checking another button automatically unchecks the previously checked one.
				p->setAutoExclusive( true );

				p_layout->addWidget( p );
				connect( p, &QPushButton::toggled, this, slot );
			};

			add_tool_button( p_layout_ctrl, p_widget_ctrl, "Редактирование", [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: edit"  ; m_view->set_tool( tool_e::edit ); } } );
			add_tool_button( p_layout_ctrl, p_widget_ctrl, "Сплошная",       [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: solid" ; m_view->set_tool( tool_e::line_solid ); } } );
			add_tool_button( p_layout_ctrl, p_widget_ctrl, "Пунктирная",     [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: dashed"; m_view->set_tool( tool_e::line_dashed ); } } );
			add_tool_button( p_layout_ctrl, p_widget_ctrl, "Размер",         [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: size"  ; m_view->set_tool( tool_e::size ); } } );
			add_tool_button( p_layout_ctrl, p_widget_ctrl, "Текст",          [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: text"  ; m_view->set_tool( tool_e::text ); } } );

			p_layout_ctrl->addWidget( new QLabel( "Размер (мм.):" ) );
			{
				QHBoxLayout * layout_ctrl_size = new QHBoxLayout;

				m_edit_width = new QLineEdit( "1000", this );
				m_edit_width->setValidator( new QIntValidator( 10/*min*/, 1999/*max*/, this ) );
				layout_ctrl_size->addWidget( m_edit_width );

				layout_ctrl_size->addWidget( new QLabel( "x", this ) );

				m_edit_height = new QLineEdit( "700", this );
				m_edit_height->setValidator( new QIntValidator( 10/*min*/, 1999/*max*/, this ) );
				layout_ctrl_size->addWidget( m_edit_height );

				p_layout_ctrl->addLayout( layout_ctrl_size );

				connect(  m_edit_width, &QLineEdit::editingFinished, this, size_changed );
				connect( m_edit_height, &QLineEdit::editingFinished, this, size_changed );
			}

			p_layout_ctrl->addWidget( new QPushButton( "Удалить", this ) );
			p_layout_ctrl->addWidget( new QPushButton( "...", this ) );
			p_layout_ctrl->addStretch( 0 );
		}

		{ // Right side
			m_scene = new QGraphicsScene( -1000/*x*/, -1000/*y*/, 2000/*w*/, 2000/*h*/, p_widget_view );
			//m_scene->setFocusOnTouch( true );
			m_view = new graphics_view( m_scene, p_widget_view );
			m_view->set_canvas_size( get_size() );
			p_central_layout->addWidget( m_view );
		}
		m_view->setFocus();
	}

protected:
	void closeEvent( QCloseEvent * /*p_event*/ ) override {
		qDebug() << __FUNCTION__;
		m_settings.save();
	}

public:
	MainWindow( QWidget * p_parent = nullptr )
		: QMainWindow( p_parent )
		, m_settings( this )
	{
		setWindowTitle( "VectorDraw" );
		if ( !m_settings.load() ) {
			// Если нет файла настроек...
			resize( 1280, 720 );

// QWidget::screen() was introduced in Qt 5.14
#if QT_VERSION >= QT_VERSION_CHECK( 5, 14, 0 )
			move( screen()->geometry().center() - frameGeometry().center() );
#endif
		}

		create_menu_bar();
		create_controls();
	}

private slots:
	void act_new() const {
		qDebug() << __FUNCTION__;
	}

	void act_save_png() const {
		util::save_png( m_view );
	}

	void act_print() const {
		QPrinter printer( QPrinter::HighResolution );
		QPainter painter( &printer );
		m_scene->render( &painter );
	}

	void size_changed() {
		m_view->set_canvas_size( get_size() );
	}
}; // class MainWindow
