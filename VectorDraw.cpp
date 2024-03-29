#include "configuration.hpp"
#include "VectorDraw.hpp"

using namespace cf_;

Configuration * Configuration::_instance = nullptr;
Configuration * cfg = Configuration::Instance();
const qint8 pen_width=2;
const QColor selected_color = Qt::green;

int main( int argc, char * argv[] ) {	
	QApplication app( argc, argv );

	MainWindow w(nullptr, argc, argv);
	
	cfg_data_type cfg_dat = {&w.get_layout(),&w,pen_width, QPen(QColor( 0, 0, 0 ),pen_width),QPen(selected_color,pen_width),
	QPen(QColor( 0, 0, 0 ),pen_width),QPen(selected_color,pen_width)
	,selected_color,QColor( 0, 0, 0 )};
	cfg->setValue(cfg_dat);
		
	w.show();
	qint16 result=app.exec();
	return result;
}

