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

	
	qint8 			m_mode; //0 - normal program start mode , 1 - start program from 1c
	QString 		m_work_path="";     //if mode==1 work path for output files  
	QString			m_filename ="";     //if mode==1 filename of output file
void parse_comandline_and_settings(int argc=0, char * argv[] = nullptr){
	//parse comand line:
	QMap<QString,QString> params;
	for (qint8 i=1; i<argc ; i+=2) 
		params[QString(argv[i])]=QString(argv[i+1]);					
	m_mode = ((params.find(QString("-f"))!=params.end())?qint8(1):qint8(0))	;		
	QString hs; 
	if ( !m_settings.load() ) {
	// if the ini file isn't exist
			resize( 1280, 720 );
			hs="100x100";	
#if QT_VERSION >= QT_VERSION_CHECK( 5, 14, 0 )
			move( screen()->geometry().center() - frameGeometry().center() );
#endif
	} else { 
			m_work_path = m_settings["path"];
			hs = m_settings["hs"] ;
	}			
				
	if (m_mode==1) { 
		hs = ((params.find(QString("-hs"))!=params.end())?QString(params["-hs"]):hs);
		m_layout.set_size(QSize((hs.left(hs.indexOf('x'))).toInt(),(hs.right(hs.size() - hs.indexOf('x')-1)).toInt()));
		m_filename = params["-f"];
	}

}	
protected:
	void closeEvent( QCloseEvent * /*p_event*/ ) override {
		m_settings.save();
	}

public:
	void set_m_work_path(QString pth) {if (m_mode==1) m_work_path = QDir::currentPath()+"/"+pth;}
	void set_m_filename(QString fln) {m_filename = fln;}
	void set_status_bar_text(QString str){m_curren_coord_sb->setText(str);}
	MainWindow( QWidget * p_parent = nullptr, int argc=0, char * argv[] = nullptr )
		: QMainWindow( p_parent )
		, m_settings( this )
		, m_menu_bar( this )
		, m_layout( this)
		, m_undo_stack( new QUndoStack( this ) )		
		,m_curren_coord_sb(new QLabel("")){
		
		statusBar()->addWidget(m_curren_coord_sb);
		setWindowTitle( "Vector Draw" );		
		parse_comandline_and_settings(argc,argv );
		if (m_mode==0)
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
		else {
		m_menu_bar.add( this, "&Файл"
			, "&Новый",				&MainWindow::act_new
			, "&Печать",			&MainWindow::act_print
			, nullptr, []{}	// separator
			, "Вы&ход",				&MainWindow::close
		);
		update_title();
		open_file(m_work_path+'/'+m_filename+".vct");
		}
		

		m_menu_bar.add( this, "&Справка"
			, "&О программе", [this]{
					QMessageBox::about( this,
						"О Vector Draw",
						"<p><b>Vector Draw</b> версия 0.1<br></p>"
						"<p><a href='https://github.com/andron81/VectorDraw'>Vector Draw на GitHub</a></p>" );
				}
			);
	}

	vd::layout& get_layout(){
		return m_layout;
	}
	void update_title() {
		setWindowTitle( "Vector Draw - " + m_filename );
	}


	void open_file(QString filename="") {	
		if (filename=="") {
		filename = QFileDialog::getOpenFileName( nullptr, "Открыть", m_work_path, "Vector Draw file (*.vct)" );
		if ( filename.isEmpty() ) return;
		} 
		m_filename = filename;
		QString val;
		QFile file;
      file.setFileName(m_filename);
      file.open(QIODevice::ReadOnly | QIODevice::Text);
      val = file.readAll();
      file.close();
      	  update_title();			  
	  get_layout().get_view()->get_m_painter().fromJson(QJsonDocument::fromJson(val.toUtf8()));				
	}	
	
	
	private slots:
	void act_new() {
		m_filename.clear();
		update_title();
		get_layout().get_view()->get_m_painter().clear_canvas();

	}
	
		void clear_scene()  {
	
		


	}
	
	void act_open() {
		
	open_file();
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
			update_title();		
	}

	void act_save_as() {
		QString filename = QFileDialog::getSaveFileName( nullptr, "Сохранить", ".", "Векторный файл (*.vct)" );
			if ( filename.isEmpty() ) return;
			m_filename = filename;
		
		QJsonDocument doc = QJsonDocument(get_layout().get_view()->get_m_painter().toJson());
		   QFile jsonFile(m_filename);
			jsonFile.open(QFile::WriteOnly);
			jsonFile.write(doc.toJson());
			update_title();
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
