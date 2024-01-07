
#include "VectorDraw.hpp"
#include "configuration.hpp"
#include "cfg_data_type.hpp"
using namespace cf_;


const qint8 pen_width=2;
const QColor selected_color = Qt::green;

int main( int argc, char * argv[] ) {	
	QApplication app( argc, argv );
	QMap<QString,QString> params;
		for (qint8 i=1; i<argc ; i+=2) 
			params[QString(argv[i])]=QString(argv[i+1]);					
		
	qint8 mode = ((params.find(QString("-f"))!=params.end())?qint8(1):qint8(0))	;
	
	QString canvas_size = ((params.find(QString("-hs"))!=params.end())?params[QString("-hs")]:"1000x700")	;
	
	MainWindow w(nullptr, mode,params["-f"]);
	cfg_data_type cfg_dat = {&w.get_layout(),&w,pen_width, QPen(QColor( 0, 0, 0 ),pen_width),QPen(selected_color,pen_width),
	QPen(QColor( 0, 0, 0 ),pen_width),QPen(selected_color,pen_width)
	,selected_color,QColor( 0, 0, 0 ),mode, canvas_size };
	Configuration<cfg_data_type>::Instance().setData(cfg_dat);
	w.show();
	qint16 result=app.exec();
	return result;
}

