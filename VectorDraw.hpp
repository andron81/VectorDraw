#pragma once

#include "VectorDraw_pch.hpp"

class MainWindow : public QMainWindow {
	Q_OBJECT

	QAction *		m_act_new;
	QAction *		m_act_exit;
	QAction *		m_act_about_qt;

	QMenu *			m_menu_file;

	template <typename SlotT>
	QAction * create_action( const QString & name, SlotT && slot ) {
		QAction * act = new QAction( name, this );
		connect( act, &QAction::triggered, this, slot );
		return act;
	}


	struct settings : QSettings {
		settings() : QSettings( QFileInfo( QCoreApplication::applicationFilePath() ).baseName() + ".ini", QSettings::IniFormat ) {}
		operator bool () const { return QFile( fileName() ).exists(); }
	}; // class settings


	bool load_settings() {
		settings s;
		s.beginGroup( "MainWindow" );
			setGeometry( s.value( "geometry" ).toRect() );
			if ( s.value( "maximized" ).toBool() ) {
				setWindowState( Qt::WindowMaximized );
			}
		s.endGroup();
		return s;
	}

	void save_settings() {
		settings s;
		s.beginGroup( "MainWindow" );
			s.setValue( "geometry", geometry() );
			s.setValue( "maximized", windowState() == Qt::WindowMaximized );
		s.endGroup();
	}

protected:
	void closeEvent( QCloseEvent * ) override {
		qDebug() << __FUNCTION__;
		save_settings();
	}

public:
	MainWindow( QWidget * parent = nullptr ) : QMainWindow( parent ) {

		setWindowTitle( "VectorDraw" );
		//setMinimumSize( 400, 300 );
		if ( !load_settings() ) {
			resize( 1024, 400 );
			move( screen()->geometry().center() - frameGeometry().center() );
		}

		// Menu bar

		// File...
		m_act_new  = create_action( tr( "&New" ) , &MainWindow::act_new  );
		m_act_exit = create_action( tr( "E&xit" ), &MainWindow::act_exit );

		m_menu_file = menuBar()->addMenu( tr( "&File" ) );
		m_menu_file->addAction( m_act_new );
		m_menu_file->addSeparator();
		m_menu_file->addAction( m_act_exit );

		// About...
		m_act_about_qt = create_action( tr( "About &Qt" ) , &MainWindow::act_about_qt );

		m_menu_file = menuBar()->addMenu( tr( "&Help" ) );
		m_menu_file->addAction( m_act_about_qt );
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

}; // class MainWindow
