#include "VectorDraw.hpp"
#include "configuration.hpp"
using namespace cf_;

Configuration * Configuration::_instance = nullptr;
Configuration * cfg = Configuration::Instance();
const qint8 pen_width=2;
int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	MainWindow w;
	cfg_data_type cfg_dat = {&w.get_layout(),&w,pen_width, QPen(QColor( 0, 0, 0 ),pen_width),QPen(QColor(0, 128, 0),pen_width)};
	cfg->setValue(cfg_dat);
	w.show();
	return app.exec();
}
