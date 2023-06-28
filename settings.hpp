#pragma once

struct settings : QSettings {
	settings()
		: QSettings(
			QFileInfo( QCoreApplication::applicationFilePath() ).baseName() + ".ini",
			QSettings::IniFormat ) {}

	operator bool () const {
		return QFile( fileName() ).exists();
	}
}; // class settings
