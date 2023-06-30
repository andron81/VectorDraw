#pragma once

#include "VectorDraw_pch.hpp"
#include "settings.hpp"

enum class tool_e { edit, line_solid, line_dashed, text, size };

class graphics_view : public QGraphicsView {
	Q_OBJECT

public:
	graphics_view( QGraphicsScene * scene, QWidget * parent )
		: QGraphicsView( scene, parent )
	{
		setDragMode( QGraphicsView::ScrollHandDrag );
		//m_view->setDragMode( QGraphicsView::RubberBandDrag );
		setInteractive( true );
		setRenderHint( QPainter::Antialiasing, true );
		setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
		setBackgroundBrush( Qt::gray );
		centerOn( 0, 0 );

		int scale = 10;//static_cast<int>(sizeSelector_->zoomFactor());

		QTransform transform;
		transform.scale( scale, scale );
		//qreal shift = 0.5 / scale;
		//transform.translate(shift, shift);
		setTransform( transform );
	}

protected:
	void wheelEvent( QWheelEvent * event ) override { qDebug() << __FUNCTION__; }
	void resizeEvent( QResizeEvent * event ) override { qDebug() << __FUNCTION__; }
	void scrollContentsBy( int dx, int dy ) override {
		//qDebug() << __FUNCTION__;
		QGraphicsView::scrollContentsBy( dx, dy );
		//lastBottomLeftPoint_ = viewport()->rect().bottomLeft();
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
		QWidget * p_central_widget = new QWidget( this );
		setCentralWidget( p_central_widget );


		QHBoxLayout * lh2 = new QHBoxLayout( p_central_widget );
		lh2->setSpacing( 0 );
		lh2->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );

		QWidget * p_wl = new QWidget( p_central_widget );
		QWidget * p_wr = new QWidget( p_central_widget );

		p_wl->setMaximumWidth( 140 );

		lh2->addWidget( p_wl );
		lh2->addWidget( p_wr );

		{ // Left side
			QVBoxLayout * lv = new QVBoxLayout( p_wl );
			lv->setObjectName( "lv" ); // For DEBUG


			auto add_tool_button = [this]( QLayout * p_layout, QWidget * p_parent, const QString & name, auto && slot ) {
				QPushButton * p = new QPushButton( name, p_parent );
				p->setCheckable( true );

				// If auto-exclusivity is enabled, checkable buttons that belong to the same parent widget behave as if they were part of the same exclusive button group.
				// In an exclusive button group, only one button can be checked at any time; checking another button automatically unchecks the previously checked one.
				p->setAutoExclusive( true );

				p_layout->addWidget( p );
				connect( p, &QPushButton::toggled, this, slot );
			};


			add_tool_button( lv, p_wl, "Редактирование", [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: edit"  ; m_tool = tool_e::edit; } } );
			add_tool_button( lv, p_wl, "Сплошная",       [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: solid" ; m_tool = tool_e::line_solid; } } );
			add_tool_button( lv, p_wl, "Пунктирная",     [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: dashed"; m_tool = tool_e::line_dashed; } } );
			add_tool_button( lv, p_wl, "Размер",         [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: size"  ; m_tool = tool_e::size; } } );
			add_tool_button( lv, p_wl, "Текст",          [&]( bool toggled ){ if ( toggled ) { qDebug() << "tool: text"  ; m_tool = tool_e::text; } } );

			lv->addWidget( new QLabel( "Размер (мм.):" ) );
			{
				QHBoxLayout * lh = new QHBoxLayout;
				lh->addWidget( new QLineEdit( "400" ) );
				lh->addWidget( new QLabel( "x" ) );
				lh->addWidget( new QLineEdit( "300" ) );
				//lh->setAlignment( Qt::AlignLeft );
				lv->addLayout( lh );
			}

			lv->addWidget( new QPushButton( "Удалить" ) );
			lv->addWidget( new QPushButton( "..." ) );
			lv->addStretch( 0 );
		}

		{ // Right side
			m_scene = new QGraphicsScene( 0/*x*/, 0/*y*/, 400/*w*/, 300/*h*/, p_wr );
			//m_scene->setStickyFocus( true );
			m_scene->setFocusOnTouch( true );
			m_view = new graphics_view( m_scene, p_wr );
			//m_scene->addItem( m_grid );
			lh2->addWidget( m_view );

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
