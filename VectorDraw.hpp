#pragma once

#include "VectorDraw_pch.hpp"
#include "vd_util.hpp"
#include "vd_settings.hpp"
#include "vd_menu_bar.hpp"
#include "vd_items.hpp"
#include "vd_view_canvas.hpp"
#include "vd_view_painter.hpp"	// tool_e
#include "vd_view.hpp"
#include "vd_layout.hpp"
#include <QFile>


// All classes that contain signals or slots must mention Q_OBJECT at the top of their declaration. They must also derive (directly or indirectly) from QObject.
class MainWindow : public QMainWindow {
	Q_OBJECT

	vd::settings	m_settings;
	vd::menu_bar	m_menu_bar;	
	vd::layout		m_layout;
	QLabel 	*		m_curren_coord_sb ; //current coord in status bar;
	QUndoStack *	m_undo_stack	= nullptr;
	QUndoView *		m_undo_view		= nullptr;

	QAction *		m_undo_action	= nullptr;
	QAction *		m_redo_action	= nullptr;

	QString			m_filename;

protected:
	void closeEvent( QCloseEvent * /*p_event*/ ) override {
		m_settings.save();
	}

public:
	void set_status_bar_text(QString str){m_curren_coord_sb->setText(str);}
	MainWindow( QWidget * p_parent = nullptr )
		: QMainWindow( p_parent )
		, m_settings( this )
		, m_menu_bar( this )
		, m_layout( this )
		, m_undo_stack( new QUndoStack( this ) )		
		 ,m_curren_coord_sb(new QLabel(""))
	{
		
		statusBar()->addWidget(m_curren_coord_sb);
			
	//m_status_bar.setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
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
			, "&Открыть",			&MainWindow::act_open
			, "&Сохранить",			&MainWindow::act_save			
			, "&Сохранить как ...",	&MainWindow::act_save_as
			, "&Экспорт"		,	&MainWindow::act_export
			, "&Печать",			&MainWindow::act_print
			, nullptr, []{}	// separator
			, "Вы&ход",				&MainWindow::close
		);


		m_menu_bar.add( this, "&Справка"
			, "&О программе", [this]{
					QMessageBox::about( this,
						"О Vector Draw",
						"<p><b>Vector Draw</b> версия 0.1<br></p>"
						"<p><a href='https://github.com/andron81/VectorDraw'>Vector Draw на GitHub</a></p>" );
				}
			, "О &Qt", &QApplication::aboutQt );

/*
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
*/
	}

	vd::layout& get_layout(){
		return m_layout;
	}
	void update_title() {
		setWindowTitle( "Vector Draw - " + m_filename );
	}
	//void edit_block_visible(bool visi) {m_layout.edit_block_visible(visi);}
private slots:
	void act_new() {
		m_filename.clear();
		update_title();
		get_layout().get_view()->get_m_painter().clear_canvas();

	}
	
		void clear_scene()  {
	
		


	}
	
	void act_open() {
		QString filename = QFileDialog::getOpenFileName( nullptr, "Сохранить", ".", "Vector Draw file (*.vct)" );
		if ( filename.isEmpty() ) return;
		m_filename = filename;
		QString val;
		QFile file;
      file.setFileName(m_filename);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      val = file.readAll();
      file.close();
      	  setWindowTitle( "Vector Draw "+m_filename );
			  
	  get_layout().get_view()->get_m_painter().fromJson(QJsonDocument::fromJson(val.toUtf8()));
	}		
	void act_save() {
		if ( m_filename.isEmpty() ) {
			QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить", ".", "Векторный файл (*.vct)" );
			if ( filename.isEmpty() ) return;
			m_filename = filename;
		}
		QJsonDocument doc = QJsonDocument(get_layout().get_view()->get_m_painter().toJson());
		   QFile jsonFile(m_filename);
			jsonFile.open(QFile::WriteOnly);
			jsonFile.write(doc.toJson());
setWindowTitle( "Vector Draw "+m_filename );			
	}

	void act_save_as() {
		QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить", ".", "Векторный файл (*.vct)" );
			if ( filename.isEmpty() ) return;
			m_filename = filename;
		
		QJsonDocument doc = QJsonDocument(get_layout().get_view()->get_m_painter().toJson());
		   QFile jsonFile(m_filename);
			jsonFile.open(QFile::WriteOnly);
			jsonFile.write(doc.toJson());
			setWindowTitle( "Vector Draw "+m_filename );
	}
	
	void act_export() {
		QFileDialog dialog(this);
		 QStringList filters;
		 filters << "jpg file (*.jpg)"<< "png file (*.png)"<< "Any files (*)";	
		 dialog.setAcceptMode (QFileDialog :: AcceptSave); 		 
		 dialog.setNameFilters(filters);		 
		 QStringList fileNames;
		 if (dialog.exec()) fileNames = dialog.selectedFiles(); else return;		 
		m_layout.get_view()->save_to_image( fileNames.back() , dialog.selectedMimeTypeFilter());
	}

	void act_print() {
		m_layout.get_view()->print();
	}
	
}; // class MainWindow
