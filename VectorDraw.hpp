#pragma once

#include "VectorDraw_pch.hpp"
#include "vd_settings.hpp"
#include "vd_menu_bar.hpp"

#include "vd_view_painter.hpp"	// tool_e
#include "vd_view_canvas.hpp"
#include "vd_view.hpp"
#include "vd_layout.hpp"

#include "vd_util.hpp"

// All classes that contain signals or slots must mention Q_OBJECT at the top of their declaration. They must also derive (directly or indirectly) from QObject.
class MainWindow : public QMainWindow {
	Q_OBJECT

	vd::settings		m_settings;
	vd::menu_bar		m_menu_bar;
	vd::layout			m_layout;

protected:
	void closeEvent( QCloseEvent * /*p_event*/ ) override {
		qDebug() << __FUNCTION__;
		m_settings.save();
	}

public:
	MainWindow( QWidget * p_parent = nullptr )
		: QMainWindow( p_parent )
		, m_settings( this )
		, m_menu_bar( this )
		, m_layout( this )
	{
		setWindowTitle( "Vector Draw" );
		if ( !m_settings.load() ) {
			// Если нет файла настроек...
			resize( 1280, 720 );

// QWidget::screen() was introduced in Qt 5.14
#if QT_VERSION >= QT_VERSION_CHECK( 5, 14, 0 )
			move( screen()->geometry().center() - frameGeometry().center() );
#endif
		}

		// Create menu bar...
		m_menu_bar.add( this, "&Файл",
			"&Новый",				&MainWindow::act_new,
			"&Сохранить как ...",	&MainWindow::act_save_image,
			"&Печать",				&MainWindow::act_print,
			nullptr, []{},	// separator
			"Вы&ход",				&MainWindow::close
		);

		m_menu_bar.add( this, "&Помощь",
			"&О программе", [this]{
					QMessageBox::about( this,
						"О VectorDraw",
						"<p><b>VectorDraw</b> версия 0.1<br></p>"
						"<p><a href='https://github.com/andron81/VectorDraw'>VectorDraw на GitHub</a></p>" );
				},
			"О &Qt", &QApplication::aboutQt );
	}

private slots:
	void act_new() const {
		qDebug() << __FUNCTION__;
	}

	void act_save_image() {
		vd::util::save_image( m_layout.get_view() );
	}

	void act_print() {
		QPrinter printer( QPrinter::HighResolution );
		QPainter painter( &printer );
		m_layout.get_scene()->render( &painter );
	}
}; // class MainWindow
