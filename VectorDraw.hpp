#pragma once

#include "VectorDraw_pch.hpp"
#include "settings.hpp"

enum class tool_e { edit, line_solid, line_dashed, text, size };

class graphics_view : public QGraphicsView {
	Q_OBJECT

	int		m_scale	= 1;

public:
	graphics_view( QGraphicsScene * scene, QWidget * parent )
		: QGraphicsView( scene, parent )
	{
		setDragMode( QGraphicsView::ScrollHandDrag );
		//m_view->setDragMode( QGraphicsView::RubberBandDrag );
		setInteractive( true );
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
		//setTransformationAnchor( QGraphicsView::AnchorUnderMouse );

		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		QTransform transform;
		transform.scale( m_scale, m_scale );
		//qreal shift = 0.5 / scale;
		//transform.translate(shift, shift);
		setTransform( transform );
	}

protected:
	void wheelEvent( QWheelEvent * event ) override {
		//qDebug() << __FUNCTION__ << ", angle: " << event->angleDelta().y() << ", pixel: " << event->pixelDelta().y();
		//if ( event->modifiers() & Qt::ShiftModifier )

		if ( event->modifiers() & Qt::ControlModifier ) {
			m_scale += event->angleDelta().y() > 0 ? 1 : -1;
			if ( m_scale <  1 ) m_scale = 1;
			if ( m_scale > 10 ) m_scale = 10;

			QTransform tform;
			tform.scale( m_scale, m_scale );
			setTransform( tform );
		}
	}

	void resizeEvent( QResizeEvent * event ) override { qDebug() << __FUNCTION__; }
	void scrollContentsBy( int dx, int dy ) override {
		//qDebug() << __FUNCTION__;
		QGraphicsView::scrollContentsBy( dx, dy );
	}
}; // class graphics_view


class MainWindow : public QMainWindow {
	Q_OBJECT

	QGraphicsScene *	m_scene;
	graphics_view *		m_view;

	tool_e				m_tool	= tool_e::edit;

	void create_menu_bar() {
		auto add_menu_action = [this]( QMenu * p_menu, const QString & name, auto && slot ) {
			QAction * p_act = new QAction( name, this );
			connect( p_act, &QAction::triggered, this, slot );
			p_menu->addAction( p_act );
		};

		QMenu * p_file = menuBar()->addMenu( "&Файл" );
		add_menu_action( p_file, "&Новый" , &MainWindow::act_new );
		add_menu_action( p_file, "&Печать", &MainWindow::act_print );
		p_file->addSeparator();
		add_menu_action( p_file, "Вы&ход" , &MainWindow::close );

		// Help
		QMenu * p_help = menuBar()->addMenu( "&Помощь" );
		add_menu_action( p_help, "О &Qt", &QApplication::aboutQt );
	}

	void create_controls() {

		// Central widget
		QWidget * widget_central = new QWidget( this );
		setCentralWidget( widget_central );


		QHBoxLayout * central_layout = new QHBoxLayout( widget_central );
		central_layout->setSpacing( 0 );
		central_layout->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );

		QWidget * widget_ctrl = new QWidget( widget_central ); widget_ctrl->setMaximumWidth( 140 );
		QWidget * widget_view = new QWidget( widget_central );

		central_layout->addWidget( widget_ctrl );
		central_layout->addWidget( widget_view );

		{ // Left side
			QVBoxLayout * layout_ctrl = new QVBoxLayout( widget_ctrl );
			layout_ctrl->setObjectName( "layout_ctrl" ); // For DEBUG

			auto add_tool_button = [this]( QLayout * p_layout, QWidget * p_parent, const QString & name, auto && slot ) {
				QPushButton * p = new QPushButton( name, p_parent );
				p->setCheckable( true );

				// If auto-exclusivity is enabled, checkable buttons that belong to the same parent widget behave as if they were part of the same exclusive button group.
				// In an exclusive button group, only one button can be checked at any time; checking another button automatically unchecks the previously checked one.
				p->setAutoExclusive( true );

				p_layout->addWidget( p );
				connect( p, &QPushButton::toggled, this, slot );
			};

			add_tool_button( layout_ctrl, widget_ctrl, "Редактирование", [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: edit"  ; m_tool = tool_e::edit; } } );
			add_tool_button( layout_ctrl, widget_ctrl, "Сплошная",       [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: solid" ; m_tool = tool_e::line_solid; } } );
			add_tool_button( layout_ctrl, widget_ctrl, "Пунктирная",     [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: dashed"; m_tool = tool_e::line_dashed; } } );
			add_tool_button( layout_ctrl, widget_ctrl, "Размер",         [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: size"  ; m_tool = tool_e::size; } } );
			add_tool_button( layout_ctrl, widget_ctrl, "Текст",          [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: text"  ; m_tool = tool_e::text; } } );

			layout_ctrl->addWidget( new QLabel( "Размер (мм.):" ) );
			{
				QHBoxLayout * layout_ctrl_size = new QHBoxLayout;

				QLineEdit * p;
				p = new QLineEdit( "400", this );
				//p->setInputMask( "D" ); // D - character of the Number category and larger than zero required, such as 1-9
				p->setValidator( new QIntValidator( 10/*min*/, 999/*max*/, this ) );
				layout_ctrl_size->addWidget( p );

				layout_ctrl_size->addWidget( new QLabel( "x", this ) );

				p = new QLineEdit( "300", this );
				p->setValidator( new QIntValidator( 10/*min*/, 999/*max*/, this ) );
				layout_ctrl_size->addWidget( p );

				//layout_ctrl_size->setAlignment( Qt::AlignLeft );
				layout_ctrl->addLayout( layout_ctrl_size );
			}

			layout_ctrl->addWidget( new QPushButton( "Удалить", this ) );
			layout_ctrl->addWidget( new QPushButton( "...", this ) );
			layout_ctrl->addStretch( 0 );
		}

		{ // Right side
			//int w = edit_w->text().toInt();

			m_scene = new QGraphicsScene( 0/*x*/, 0/*y*/, 100/*w*/, 100/*h*/, widget_view );
			m_scene->setFocusOnTouch( true );
			m_view = new graphics_view( m_scene, widget_view );
			//m_scene->addItem( m_grid );
			central_layout->addWidget( m_view );

		}
	}

protected:
	void closeEvent( QCloseEvent * /*p_event*/ ) override {
		qDebug() << __FUNCTION__;
		settings s;
		s.save( this );
	}

public:
	MainWindow( QWidget * parent = nullptr ) : QMainWindow( parent ) {
		setWindowTitle( "VectorDraw" );

		settings s;
		if ( !s.load( this ) ) {
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
	void act_new() {
		qDebug() << __FUNCTION__;
	}

	void act_print() {
		QPrinter printer( QPrinter::HighResolution );
		//printer.setPaperSize( QPrinter::A4 );
		QPainter painter( &printer );
		m_scene->render( &painter );
	}

	void line_solid() {
		//QGraphicsLineItem * p = m_scene->addLine( 100, 100, 300, 200 );
		QGraphicsLineItem * p = m_scene->addLine( 100, 100, 300, 200, QPen( Qt::DashLine ) );
		p->setAcceptHoverEvents( true );
		p->setAcceptTouchEvents( true );
		p->setFlags( QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable );
		//stackBefore(const QGraphicsItem *sibling)
		//m_scene->setFocusItem( p );
	}

private:
#if 0
	template <typename SlotT>
	QPushButton * create_button( const QString & name, SlotT && slot ) {
		QPushButton * p = new QPushButton( name, this );
		connect( p, &QPushButton::clicked, this, slot );
		return p;
	}
#endif
}; // class MainWindow
