#pragma once

#include "VectorDraw_pch.hpp"
#include "settings.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT

	QAction *			m_act_new;
	QAction *			m_act_exit;
	QAction *			m_act_about_qt;

	QGraphicsScene *	m_scene;
	QGraphicsView *		m_view;

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
			//move( screen()->geometry().center() - frameGeometry().center() );
		}


		// Create menu actions...
		m_act_new		= create_action( "&Новый", &MainWindow::act_new  );
		m_act_exit		= create_action( "Вы&ход", &MainWindow::act_exit );
		m_act_about_qt	= create_action(  "О &Qt", &MainWindow::act_about_qt );


		// File...
		{
			QMenu * p = menuBar()->addMenu( "&Файл" );
			p->addAction( m_act_new );
			p->addSeparator();
			p->addAction( m_act_exit );
		}

		// Help...
		{
			QMenu * p = menuBar()->addMenu( "&Помощь" );
			p->addAction( m_act_about_qt );
		}



		// Central widget
		QWidget * w = new QWidget( this );
		setCentralWidget( w );
		{
			QHBoxLayout * lh2 = new QHBoxLayout( w );
			lh2->setSpacing( 0 );
			lh2->setContentsMargins( 0/*left*/, 0/*top*/, 4/*right*/, 4/*bottom*/ );

			QWidget * p_wl = new QWidget( w );
			QWidget * p_wr = new QWidget( w );

			p_wl->setMaximumWidth( 140 );

			lh2->addWidget( p_wl );
			lh2->addWidget( p_wr );


			{ // Left side
				QVBoxLayout * lv = new QVBoxLayout( p_wl );
				lv->setObjectName( "lv" ); // For DEBUG
				lv->addWidget( new QPushButton( "Сплошная" ) );
				lv->addWidget( new QPushButton( "Пунктирная" ) );
				lv->addWidget( new QPushButton( "Размер" ) );
				lv->addWidget( new QPushButton( "Текст" ) );

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
				m_scene = new QGraphicsScene( p_wr );
				m_view = new QGraphicsView( p_wr );

				m_view->setRenderHint( QPainter::Antialiasing, true );
				m_view->setViewportUpdateMode( QGraphicsView::SmartViewportUpdate );
				m_view->setScene( m_scene );
				m_view->setBackgroundBrush( Qt::gray );

				lh2->addWidget( m_view );
			}
		}
	}

private slots:
	void act_new() {
		qDebug() << __FUNCTION__;
	}

	void act_exit() {
		qDebug() << __FUNCTION__;
		close();
	}

	void act_about_qt() {
		QApplication::aboutQt();
	}

private:

	// Create action and connect it to slot...
	template <typename SlotT>
	QAction * create_action( const QString & name, SlotT && slot ) {
		QAction * act = new QAction( name, this );
		connect( act, &QAction::triggered, this, slot );
		return act;
	}
}; // class MainWindow
