#include "VectorDraw.hpp"
#include "configuration.hpp"
using namespace cf_;

Configuration * Configuration::_instance = nullptr;
Configuration * cfg = Configuration::Instance();

int main( int argc, char * argv[] ) {
	QApplication app( argc, argv );
	MainWindow w;
	cfg_data_type cfg_dat = {&w.get_layout(), QColor( 0, 255, 0 )};
	cfg->setValue(cfg_dat);
	w.show();
	return app.exec();
}
