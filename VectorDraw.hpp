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

	vd::settings	m_settings;
	vd::menu_bar	m_menu_bar;
	vd::layout		m_layout;

	QUndoStack *	m_undo_stack	= nullptr;
	QUndoView *		m_undo_view		= nullptr;

	QAction *		m_undo_action	= nullptr;
	QAction *		m_redo_action	= nullptr;

protected:
	void closeEvent( QCloseEvent * /*p_event*/ ) override {
		m_settings.save();
	}

public:
	MainWindow( QWidget * p_parent = nullptr )
		: QMainWindow( p_parent )
		, m_settings( this )
		, m_menu_bar( this )
		, m_layout( this )
		, m_undo_stack( new QUndoStack( this ) )
	{
		//statusBar()->showMessage( "Ready" );

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
		m_menu_bar.add( this, "&Файл"
			, "&Новый",				&MainWindow::act_new
			, "&Сохранить как ...",	&MainWindow::act_save_image
			, "&Печать",			&MainWindow::act_print
			, nullptr, []{}	// separator
			, "Вы&ход",				&MainWindow::close
		);

		m_menu_bar.add( this, "&Правка"
			, "&Отменить",	[this]{ qDebug() << "Menu: undo (TODO)"; }
			, "&Повторить",	[this]{ qDebug() << "Menu: redo (TODO)"; }
		);

		m_menu_bar.add( this, "&Справка"
			, "&О программе", [this]{
					QMessageBox::about( this,
						"О Vector Draw",
						"<p><b>Vector Draw</b> версия 0.1<br></p>"
						"<p><a href='https://github.com/andron81/VectorDraw'>Vector Draw на GitHub</a></p>" );
				}
			, "О &Qt", &QApplication::aboutQt );


		// Test Undo Stack
#ifndef NDEBUG
		QDockWidget * p_undo_dock_widget = new QDockWidget;
		p_undo_dock_widget->setWindowTitle( "Command list" );
		p_undo_dock_widget->setWidget( new QUndoView( m_undo_stack ) );
		addDockWidget( Qt::RightDockWidgetArea, p_undo_dock_widget );
#endif // NDEBUG

		m_undo_action = m_undo_stack->createUndoAction( this, "&Undo" );
		m_undo_action->setIcon( QIcon( ":/images/edit_undo.png" ) );
		m_undo_action->setShortcuts( QKeySequence::Undo );

		m_redo_action = m_undo_stack->createRedoAction( this, "&Redo" );
		m_redo_action->setIcon( QIcon( ":/images/edit_redo.png" ) );
		m_redo_action->setShortcuts( QKeySequence::Redo );

		//undoStack->push(new MoveCommand(movedItem, oldPosition));
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
