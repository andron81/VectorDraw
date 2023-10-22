#pragma once
#define CONFIGURATION_H
#include "vd_layout.hpp"

namespace cf_ {
struct cfg_data_type{
	vd::layout* layout;
	QPen m_focus_line_pen = QColor( 0, 255, 0 );	
};	
class Configuration
{
    static Configuration *_instance;
    cfg_data_type  _savedValue ;

public:

    static Configuration* Instance();
	
	
    cfg_data_type getSavedValue();
	void setValue(cfg_data_type data) ;
private:
    Configuration();
};
}

