#pragma once

#include "VectorDraw_pch.hpp"

class settings : QSettings {
	Q_OBJECT

public:
	settings()
		: QSettings(
			QFileInfo( QCoreApplication::applicationFilePath() ).baseName() + ".ini",
			QSettings::IniFormat ) {}

	bool load( QMainWindow * p_main_window ) {
		//settings s;
		beginGroup( "MainWindow" );
			p_main_window->setGeometry( value( "geometry" ).toRect() );
			if ( value( "maximized" ).toBool() ) {
				p_main_window->setWindowState( Qt::WindowMaximized );
			}
		endGroup();
		return QFile( fileName() ).exists();
	}

	void save( QMainWindow * p_main_window ) {
		beginGroup( "MainWindow" );
			setValue( "geometry", p_main_window->geometry() );
			setValue( "maximized", p_main_window->windowState() == Qt::WindowMaximized );
		endGroup();
	}
}; // class settings
