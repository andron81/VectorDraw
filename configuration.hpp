#pragma once
#include "vd_layout.hpp"

namespace cf_ {
struct cfg_data_type{
	vd::layout* layout;
	qint8 pen_width;
	QPen m_line_pen;
	QPen m_focused_line_pen;

};	
class Configuration
{
    static Configuration *_instance;
    cfg_data_type  _savedValue ;

public:

    static Configuration* Instance();
	
	
    cfg_data_type& getSavedValue();
	void setValue(cfg_data_type data) ;
private:
    Configuration();
};
};

