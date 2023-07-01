﻿#pragma once

#include "VectorDraw_pch.hpp"

class settings : QSettings {
	QMainWindow *	m_main_window;

public:
	settings( QMainWindow *	p_main_window )
		: QSettings( QFileInfo( QCoreApplication::applicationFilePath() ).baseName() + ".ini", QSettings::IniFormat )
		, m_main_window( p_main_window ) { Q_ASSERT( m_main_window ); }

	bool load() {
		beginGroup( "MainWindow" );
			m_main_window->setGeometry( value( "geometry" ).toRect() );
			if ( value( "maximized" ).toBool() ) {
				m_main_window->setWindowState( Qt::WindowMaximized );
			}
		endGroup();
		return QFile( fileName() ).exists();
	}

	void save() {
		beginGroup( "MainWindow" );
			setValue( "geometry", m_main_window->geometry() );
			setValue( "maximized", m_main_window->windowState() == Qt::WindowMaximized );
		endGroup();
	}
}; // class settings
